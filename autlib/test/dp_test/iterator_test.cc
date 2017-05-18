/* ***********************************************************************

  > File Name: iterator_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月18日 星期四 14时04分54秒

 ********************************************************************** */

// 迭代器模式

#include <iostream>

// 抽象迭代器
template <typename T>
class IteratorBase
{
public:
    virtual ~IteratorBase() {}
    virtual T operator*() = 0;
    virtual T operator++() = 0;
};

// 抽象聚合类
template <typename T>
class ObjectBase
{
public:
    virtual ~ObjectBase() {}
    virtual IteratorBase *createIterator() = 0;
    virtual void addObject(const T &obj) { m_object.insert(obj); }
    virtual void removeObject(const T &obj) { m_object.erase(obj); }

private:
    T m_object;
};

// 具体迭代器

// 具体聚合类

int main(int argc, const char* argv[])
{

    return 0;
}
