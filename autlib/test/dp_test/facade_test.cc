/* ***********************************************************************

  > File Name: facade_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月16日 星期二 11时40分24秒

 ********************************************************************** */

// 外观模式 - 包含抽象外观类

#include <iostream>

// 抽象外观类
class FacadeBase
{
public:
    virtual ~FacadeBase() {}
    virtual void operate() = 0;
};

// 子系统类1
class SubSystem1
{
public:
    void function() { std::cout << "SubSystem1::function()" << std::endl; }
};

// 子系统类2
class SubSystem2
{
public:
    void function() { std::cout << "SubSystem2::function()" << std::endl; }
};

// 具体外观类
class Facade : public FacadeBase
{
public:
    void operate() 
    {
        ss1.function();
        ss2.function();
    }

private:
    SubSystem1 ss1;
    SubSystem2 ss2;
};

int main(int argc, const char* argv[])
{
    FacadeBase *f = new Facade;
    f->operate();
    delete f;

    return 0;
}
