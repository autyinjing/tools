/* ***********************************************************************

  > File Name: decorator_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月12日 星期五 16时48分53秒

 ********************************************************************** */

// 装饰模式 - 使用透明装饰模式

#include <iostream>
#include <string>

// 抽象构件类
class StringEncryptBase
{
public:
    virtual ~StringEncryptBase() {}
    virtual std::string encrypt(const std::string &srcStr) = 0;
};

// 具体构件类 - 普通加密
class StringEncrypt : public StringEncryptBase
{
public:
    std::string encrypt(const std::string &srcStr)
    {
        std::cout << "encrypt from StringEncrypt" << std::endl;
        return std::string("");
    }
};

// 抽象装饰类
class ExtraStringEncryptBase : public StringEncryptBase
{
public:
    ExtraStringEncryptBase(StringEncryptBase *seb) : m_seb(seb) {}
    virtual ~ExtraStringEncryptBase() { m_seb = nullptr; }

    std::string encrypt(const std::string &srcStr) 
    {
        return this->extraEncrypt(m_seb->encrypt(srcStr));
    }
    virtual std::string extraEncrypt(const std::string &srcStr) { return std::string(""); }

protected:
    StringEncryptBase *m_seb;
};

// 具体装饰类 - 取模加密
class StringModEncrypt : public ExtraStringEncryptBase
{
public:
    StringModEncrypt(StringEncryptBase *seb) : ExtraStringEncryptBase(seb) {}
    std::string extraEncrypt(const std::string &srcStr)
    {
        std::cout << "encrypt from StringModEncrypt" << std::endl;
        return std::string("");
    }
};

// 具体装饰类 - 逆向输出加密
class StringReverseEncrypt : public ExtraStringEncryptBase
{
public:
    StringReverseEncrypt(StringEncryptBase *seb) : ExtraStringEncryptBase(seb) {}
    std::string extraEncrypt(const std::string &srcStr)
    {
        std::cout << "encrypt from StringReverseEncrypt" << std::endl;
        return std::string("");
    }
};

int main()
{
    StringEncryptBase *seb    = new StringEncrypt;
    StringEncryptBase *eseb   = new StringReverseEncrypt(seb);
    StringEncryptBase *eeseb  = new StringModEncrypt(eseb);
    StringEncryptBase *eeeseb = new StringModEncrypt(eeseb);

    eseb->encrypt(std::string(""));
    eeseb->encrypt(std::string(""));
    eeeseb->encrypt(std::string(""));

    delete seb;
    delete eseb;
    delete eeseb;
    delete eeeseb;

    return 0;
}
