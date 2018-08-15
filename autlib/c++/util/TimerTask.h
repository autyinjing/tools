
// Must compile with C++11

#ifndef TIMERTASK_H
#define TIMERTASK_H


#include <stdint.h>
#include <map>
#include <functional>
#include <set>

typedef std::function<void()> TaskFunc;

class ITimerTask;
class PriorityQueueTimerTask;
class BaseTask
{
    friend class PriorityQueueTimerTask;

private:
    std::set<int64_t> ids_;
    PriorityQueueTimerTask *timer_task_;
public:
    BaseTask()
    {
        timer_task_ = nullptr;
        ids_.clear();
    }

    void InitTimerTask(PriorityQueueTimerTask *timer_task)
    {
        timer_task_ = timer_task;
    }

    ~BaseTask();
};

//定时器接口
class ITimerTask
{
public:
    virtual ~ITimerTask(){};

    //以当前时间运行，所有小于time的任务都会被运行
    //保证按时间运行，不保证同一时间的任务顺序
    virtual void Run(int64_t time) = 0;


    /*
    //添加一个任务
    //返回id
    //会拷贝一份task
    template <class T>
    virtual int64_t AddTask(int64_t time, T &task,void (T::*func)() ) = 0;
     */

    //删除任务。被删除的任务不再会被运行
    //任务不存在返回false
    virtual bool DelTask(int64_t taskId) = 0;

    //重置任务时间，指定的任务会以新的时间运行
    virtual bool ResetTaskTime(int64_t time, int64_t taskId) = 0;
};

class __Task{
public:
//    ITask* task_;
    int64_t id_;
    int64_t runtime_;
    BaseTask* base_task_;
//    ITaskFunc func;

    TaskFunc func;

    __Task(){}
    ~__Task(){}
};

class PriorityQueueTimerTask : public ITimerTask
{
    friend class BaseTask;
private:
    std::multimap<int64_t, __Task *> tasks_;
    std::map<int64_t, std::multimap<int64_t, __Task * >::iterator > id_2_task_;
    int64_t id = 0;
    int64_t next_id()
    {
        id++;
        return id;
    }

public:
    PriorityQueueTimerTask() = default;

    virtual ~PriorityQueueTimerTask() = default;

    void Run(int64_t time) override
    {
        while (true)
        {
            auto it = tasks_.begin();
            if (it == tasks_.end())
            {
                break;
            }
            if(it->first <= time)
            {
                //(it->second->task_->*(it->second->func))();

                id_2_task_.erase(it->second->id_);
                it->second->func();
                delete it->second;
                tasks_.erase(it);
            }
            else
            {
                break;
            }
        }
    }
    template <class T>
    int64_t AddTask(int64_t time, T* task, std::function<void()>&& func)
    {
        __Task *aTask = new __Task();
        aTask->id_ = next_id();
        aTask->runtime_ = time;
        aTask->func = std::move(func);
        auto it = tasks_.insert(std::pair<int64_t, __Task *>(time, aTask));
        id_2_task_[aTask->id_] = it;

        BaseTask* baseTask = dynamic_cast<BaseTask*>(task);
        if(baseTask!= nullptr)
        {
            baseTask->ids_.insert(aTask->id_);
        }
        aTask->base_task_ = baseTask;

        return aTask->id_;
    }

    template <class T>
    int64_t AddTask(int64_t time, T *task,void (T::*func)() )
    {
        __Task *aTask = new __Task();
        aTask->id_ = next_id();
        aTask->runtime_ = time;
        aTask->func = std::bind(func,task);
        auto it = tasks_.insert(std::pair<int64_t, __Task *>(time, aTask));
        id_2_task_[aTask->id_] = it;

        BaseTask* baseTask = dynamic_cast<BaseTask*>(task);
        if(baseTask!= nullptr)
        {
            baseTask->ids_.insert(aTask->id_);
        }
        aTask->base_task_ = baseTask;

        return aTask->id_;
    }

    bool DelTask(int64_t taskId) override
    {
        auto it = id_2_task_.find(taskId);
        if(it == id_2_task_.end())
        {
            return false;
        }
        __Task * task = it->second->second;
        tasks_.erase(it->second);
        id_2_task_.erase(it);

        if(nullptr != task->base_task_)
        {
            task->base_task_->ids_.erase(taskId);
        }

        delete task;
        return true;
    }

    bool ResetTaskTime(int64_t time, int64_t taskId) override
    {
        auto it = id_2_task_.find(taskId);
        if(it == id_2_task_.end())
        {
            return false;
        }
        __Task * task = it->second->second;
        tasks_.erase(it->second);
        id_2_task_.erase(it);
        task->runtime_ = time;
        auto new_it = tasks_.insert(std::pair<int64_t, __Task *>(time, task));
        id_2_task_[task->id_] = new_it;
        return true;
    }
private:
    bool DelSelfTask(int64_t taskId)
    {
        auto it = id_2_task_.find(taskId);
        if(it == id_2_task_.end())
        {
            return false;
        }
        __Task * task = it->second->second;
        tasks_.erase(it->second);
        id_2_task_.erase(it);
        delete task;
        return true;
    }
};

BaseTask::~BaseTask()
{
    if (timer_task_ != nullptr)
    {
        for (int64_t i : ids_)
        {
            timer_task_->DelSelfTask(i);
        }
        ids_.clear();
    }
}

#endif //TIMERTASK_H
