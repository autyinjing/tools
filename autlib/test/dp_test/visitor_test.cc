/* ***********************************************************************

  > File Name: visitor_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月25日 星期四 15时53分12秒

 ********************************************************************** */

// 访问者模式

#include <iostream>
#include <string>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::set;

class Teacher;
class Student;

// 抽象访问者
class Evaluate
{
public:
    virtual ~Evaluate() {}
    virtual void visit(Teacher *t) = 0;
    virtual void visit(Student *s) = 0;
};

// 抽象元素
class Role
{
public:
    virtual ~Role() {}
    virtual void accept(Evaluate *ev) = 0;
};

// 具体元素 - 教师
class Teacher : public Role
{
public:
    void accept(Evaluate *ev) { ev->visit(this); }
};

// 具体元素 - 学生
class Student : public Role
{
public:
    void accept(Evaluate *ev) { ev->visit(this); }
};

// 具体访问者 - 论文审批
class PaperEvaluate : public Evaluate
{
public:
    void visit(Teacher *t) { cout << "PaperEvaluate visit teacher !\n"; }
    void visit(Student *s) { cout << "PaperEvaluate visit student !\n"; };
};

// 具体访问者 - 成绩评定
class ScoreEvaluate : public Evaluate
{
public:
    void visit(Teacher *t) { cout << "ScoreEvaluate visit teacher !\n"; }
    void visit(Student *s) { cout << "ScoreEvaluate visit student !\n"; };
};

// 对象结构 - 审批系统
class EvaluateSystem
{
public:
    void addRole(Role *r) { roles.insert(r); }
    void accept(Evaluate *ev) { for (auto it : roles) it->accept(ev); }

private:
    set<Role *> roles;
};

int main(int argc, const char* argv[])
{
    EvaluateSystem es;
    Role *t = new Teacher;
    Role *s = new Student;
    Evaluate *pe = new PaperEvaluate;
    Evaluate *se = new ScoreEvaluate;

    es.addRole(t);
    es.addRole(s);
    es.accept(pe);
    es.accept(se);

    delete t;
    delete s;
    delete pe;
    delete se;

    return 0;
}
