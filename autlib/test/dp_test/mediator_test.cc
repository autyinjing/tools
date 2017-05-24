/* ***********************************************************************

  > File Name: mediator_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月24日 星期三 14时40分22秒

 ********************************************************************** */

// 中介者模式

#include <iostream>

class ComponentBase;

using std::cout;
using std::endl;

// 抽象中介者
class MediatorBase
{
public:
    virtual ~MediatorBase() {}
    virtual void componentChanged(ComponentBase *c) = 0;
};

// 抽象同事类
class ComponentBase
{
public:
    virtual ~ComponentBase() {}
    void changed() { if (mediator != nullptr) mediator->componentChanged(this); }
    void setMediator(MediatorBase *mb) { mediator = mb; }

    virtual void update() {}
    virtual void select() {}
    virtual void setText() {}

protected:
    MediatorBase *mediator;
};

// 具体中介者
class Mediator : public MediatorBase
{
public:
    ComponentBase *btn, *lst, *tbx, *cbx;

    void componentChanged(ComponentBase *c)
    {
        if (c == btn) {
            cout << "-- 单击增加按钮 --" << endl;
            lst->update();
            cbx->update();
            tbx->update();
        } else if (c == lst) {
            cout << "-- 从列表框选择客户 --" << endl;
            cbx->select();
            tbx->setText();
        } else if (c == cbx) {
            cout << "-- 从组合框选择客户 --" << endl;
            cbx->select();
            tbx->setText();
        }
    }
};

// 具体同事类
class Button : public ComponentBase
{
public:
    void update() {}
};

// 具体同事类
class List : public ComponentBase
{
public:
    void update() { cout << "列表增加一项！" << endl; }
    void select() { cout << "列表选择一项！" << endl; }
};

// 具体同事类
class TextBox : public ComponentBase
{
public:
    void update() { cout << "增加成功！文本框清空" << endl; }
    void setText() { cout << "文本框设置文字！" << endl; }
};

// 具体同事类
class ComboBox : public ComponentBase
{
public:
    void update() { cout << "组合框增加一项！" << endl; }
    void select() { cout << "组合框选中一项！" << endl; }
};

int main(int argc, const char* argv[])
{
    ComponentBase *btn = new Button;
    ComponentBase *lst = new List;
    ComponentBase *tbx = new TextBox;
    ComponentBase *cbx = new ComboBox;
    Mediator *mb = new Mediator;

    mb->btn = btn;
    mb->lst = lst;
    mb->tbx = tbx;
    mb->cbx = cbx;
    btn->setMediator(mb);
    lst->setMediator(mb);
    tbx->setMediator(mb);
    cbx->setMediator(mb);

    btn->changed();
    lst->changed();

    delete btn;
    delete lst;
    delete tbx;
    delete cbx;
    delete mb;

    return 0;
}
