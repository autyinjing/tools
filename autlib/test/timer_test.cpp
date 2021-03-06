/* ***********************************************************************

  > File Name: test.cpp
  > Author: Aut
  > Mail: autyinjing@126.com 
  > Created Time: 2018年08月15日 星期三 11时55分09秒

 ********************************************************************** */

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include "../c++/util/TimerTask.h"

class A : public BaseTask
{
public:
    A()
    {
        timer_task_ = new PriorityQueueTimerTask();
        InitTimerTask(timer_task_);
    }
    template <class T>
    int64_t AddTask(double delay,T* task,void (T::*func)() )
    {
        int64_t now = time(NULL);
        return timer_task_->AddTask((int64_t)((now+delay)*1000000) , task,func);
    }

    template <class T>
    int64_t AddTaskWithContext(double delay,T* task,void (T::*func)(const std::string&), const std::string &context)
    {
        int64_t now = time(NULL);
        return timer_task_->AddTaskWithContext((int64_t)((now+delay)*1000000), task, func, context);
    }

    virtual void OnTick()
    {
        timer_task_->Run(time(NULL)*1000000);
        timer_task_->RunWithContext(time(NULL)*1000000);
    }
    void On10s()
    {
        std::cout << "On10s" << std::endl;
        AddTask(10, this, &A::On10s);
    }
    void On6s()
    {
        std::cout << "On6s" << std::endl;
        AddTask(6, this, &A::On6s);
    }
    void On11s(const std::string &context)
    {
        std::cout << "context: " << context << std::endl;
        AddTaskWithContext(11, this, &A::On11s, context+" aut");
    }

private:
    PriorityQueueTimerTask* timer_task_;
};

int main(int argc, const char* argv[])
{
    A a, b;
    a.AddTask(10, &a, &A::On10s);
    a.AddTask(6, &a, &A::On6s);
    a.AddTaskWithContext(11, &a, &A::On11s, "aut");
    while (true)
    {
        struct timeval tm;
        tm.tv_sec = 0;
        tm.tv_usec = 50;
        select(1, nullptr, nullptr, nullptr, &tm);
        a.OnTick();
    }

    return 0;
}

