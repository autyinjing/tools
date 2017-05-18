/* ***********************************************************************

  > File Name: template_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月18日 星期四 16时26分01秒

 ********************************************************************** */

// 模板方法模式

#include <iostream>
#include <string>

// 抽象类 - 数据查看类
class DataViewerBase
{
public:
    virtual ~DataViewerBase() {}
    void view(std::string src) 
    { 
        std::string data = readData(src); 
        if (needTrans(src))
            data = transData(data);
        displayData(data);
    }
    virtual std::string readData(std::string src) = 0;
    virtual bool needTrans(std::string src) { return false; }
    virtual std::string transData(std::string data) = 0;
    virtual void displayData(std::string data) = 0;
};

// 具体子类1
class DataProcessA : public DataViewerBase
{
public:
    std::string readData(std::string src) 
    { 
        std::cout << "DataProcessA::readData data: " << src << std::endl; 
        return src;
    }
    std::string transData(std::string data) { return data; }
    void displayData(std::string data)
    {
        std::cout << "DataProcessA::display data: " << data << std::endl;
    }
};

// 具体子类2
class DataProcessB : public DataViewerBase
{
public:
    std::string readData(std::string src) 
    { 
        std::cout << "DataProcessB::readData data: " << src << std::endl; 
        return src;
    }
    std::string transData(std::string data) { return data + "111"; }
    bool needTrans(std::string src) { return true; }
    void displayData(std::string data)
    {
        std::cout << "DataProcessB::display data: " << data << std::endl;
    }
};

int main(int argc, const char* argv[])
{
    DataViewerBase *dpa = new DataProcessA;
    DataViewerBase *dpb = new DataProcessB;

    dpa->view("aut");
    dpb->view("honey");

    delete dpa;
    delete dpb;

    return 0;
}
