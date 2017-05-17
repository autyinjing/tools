/* ***********************************************************************

  > File Name: cor_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月17日 星期三 16时47分39秒

 ********************************************************************** */

// 职责链模式 - 纯的职责链模式

#include <iostream>

// 抽象处理者 - 审批者
class ApproverBase
{
public:
    ApproverBase() { m_nextApp = nullptr; }
    virtual ~ApproverBase() {}
    virtual bool handleRequest(uint32_t days) = 0;
    void setNext(ApproverBase *next) { m_nextApp = next; }

protected:
    ApproverBase *m_nextApp;
};

// 具体处理者 - 主任
class Director : public ApproverBase
{
public:
    bool handleRequest(uint32_t days)
    {
        if (days < 3) {
            std::cout << "Director agree" << std::endl;
            return true;
        } else if (m_nextApp != nullptr) {
            return m_nextApp->handleRequest(days);
        }
        return false;
    }
};

// 具体处理者 - 经理
class Manager : public ApproverBase
{
public:
    bool handleRequest(uint32_t days)
    {
        if (days < 10) {
            std::cout << "Manager agree" << std::endl;
            return true;
        } else if (m_nextApp != nullptr) {
            return m_nextApp->handleRequest(days);
        }
        return false;
    }
};

// 具体处理者 - 总经理
class GManager : public ApproverBase
{
public:
    bool handleRequest(uint32_t days)
    {
        if (days < 30) {
            std::cout << "GManager agree" << std::endl;
            return true;
        } else if (m_nextApp != nullptr) {
            return m_nextApp->handleRequest(days);
        }
        std::cout << "GManager disagree" << std::endl;
        return false;
    }
};

int main(int argc, const char* argv[])
{
    ApproverBase *pDir = new Director();
    ApproverBase *pMan = new Manager();
    ApproverBase *pGma = new GManager();

    pDir->setNext(pMan);
    pMan->setNext(pGma);

    pDir->handleRequest(2);
    pDir->handleRequest(3);
    pDir->handleRequest(10);
    pDir->handleRequest(30);

    delete pDir;
    delete pMan;
    delete pGma;

    return 0;
}
