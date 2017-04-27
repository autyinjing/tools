/* ***********************************************************************

  > File Name: singleton.hpp
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年04月27日 星期四 16时32分41秒

 ********************************************************************** */

#ifndef AUT_SINGLETON_HPP
#define AUT_SINGLETON_HPP

// 注：线程不安全，使用时需要在程序启动时手动实例化

template <class T>
class Singleton
{
public:
    static T& get_instance()
    {
        if (m_singleton == nullptr)
            m_singleton = new T;
        return *m_singleton;
    }

    static T* get_instance_ptr()
    {
        if (m_singleton == nullptr)
            m_singleton = new T;
        return m_singleton;
    }

    static void del_instance()
    {
        if (m_singleton != nullptr) {
            delete m_singleton;
            m_singleton = nullptr;
        }
    }

protected:
    Singleton() {}  //防止外部创建对象

private:
    static T* m_singleton;
};

template <class T> 
T* Singleton<T>::m_singleton;

#endif      //end of AUT_SINGLETON_HPP
