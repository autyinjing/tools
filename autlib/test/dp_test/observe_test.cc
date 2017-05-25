/* ***********************************************************************

  > File Name: observe_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月24日 星期三 18时05分04秒

 ********************************************************************** */

// 观察者模式

#include <iostream>
#include <set>
#include <string>

using std::cout;
using std::endl;
using std::set;
using std::string;

// 观察者
class Observer
{
public:
    virtual ~Observer() {}
    virtual void update(int price) = 0;
};

// 具体观察者
class Player : public Observer
{
public:
    Player(string na) : name(na) {}
    void update(int price) { cout << name << ", new price: " << price << endl; }

private:
    string name;
};

// 目标
class SubjectBase
{
public:
    virtual ~SubjectBase() {}
    void join(Observer *p) { players.insert(p); }
    void quit(Observer *p) { players.erase(p); }
    virtual void notify(int price) = 0;

protected:
    set<Observer *> players;
};

// 具体目标
class RealSubject : public SubjectBase
{
public:
    void notify(int price) { for (auto it : players) it->update(price); }
    void change(int price) { notify(price); }
};

int main(int argc, const char* argv[])
{
    Observer *p1 = new Player("aut");
    Observer *p2 = new Player("honey");

    RealSubject rs;
    rs.join(p1);
    rs.join(p2);
    rs.change(12);
    rs.change(87);

    delete p1;
    delete p2;

    return 0;
}
