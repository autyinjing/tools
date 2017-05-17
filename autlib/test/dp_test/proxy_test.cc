/* ***********************************************************************

  > File Name: proxy_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月17日 星期三 11时40分57秒

 ********************************************************************** */

// 代理模式 - 保护代理和智能引用代理

#include <iostream>
#include <string>
#include <map>

// 抽象主题角色 - 抽象查询类
class SearchBase
{
public:
    virtual ~SearchBase() {}
    virtual std::string doSearch(uint32_t uid, std::string key) = 0;
};

// 真实主题角色 - 具体查询类
class RealSearch : public SearchBase
{
public:
    std::string doSearch(uint32_t uid, std::string key) 
    { 
        std::cout << "doSearch from RealSearch" << std::endl;
        std::cout << "uid: " << uid << "  key: " << key << std::endl;
        return std::string(std::to_string(uid-100000)); 
    }
};

// 业务类 - 权限控制
class AccessValidator
{
public:
    bool isValidating(uint32_t uid) { return uid == 101219; }
};

// 业务类 - 日志记录
class Logger
{
public:
    void log(uint32_t uid, std::string log_info, std::string result)
    { 
        std::cout << uid << " " << log_info << " ret: " << result << std::endl;
        ++m_logCnt[uid];
    }
    uint32_t getLogCnt(uint32_t uid)
    { 
        std::map<uint32_t, uint32_t>::iterator it = m_logCnt.find(uid);
        if (it != m_logCnt.end())
            return it->second;
        return 0; 
    }

private:
    std::map<uint32_t, uint32_t> m_logCnt;
};

// 代理主题角色 - 代理查询类
class SearchProxy : public SearchBase
{
public:
    std::string doSearch(uint32_t uid, std::string key)
    {
        std::string ret;
        if (m_validator.isValidating(uid))
            ret = m_searcher.doSearch(uid, key);
        m_logger.log(uid, "执行了查询语句：" + key, ret);
        return ret;
    }

private:
    AccessValidator m_validator;
    Logger      m_logger;
    RealSearch  m_searcher;
};

int main(int argc, const char* argv[])
{
    SearchBase *sb  = new SearchProxy;
    SearchBase *rsb = new RealSearch;
    sb->doSearch(101219, "select * from DB.table");
    sb->doSearch(100087, "select * from DB.table");
    rsb->doSearch(100087, "select * from DB.table");
    delete sb;
    delete rsb;
    sb  = nullptr;
    rsb = nullptr;

    return 0;
}
