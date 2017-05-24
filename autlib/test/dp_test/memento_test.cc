/* ***********************************************************************

  > File Name: memento_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月24日 星期三 16时10分34秒

 ********************************************************************** */

// 备忘录模式

#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

// 备忘录类 - 棋子备忘录
class ChessmanMemento
{
public:
    friend class Chessman;
    ChessmanMemento() {}

private:
    ChessmanMemento(string l, int xx, int yy)
        : label(l), x(xx), y(yy) {}
    string getLabel() { return label; }
    int getX() { return x; }
    int getY() { return y; }

    string label;
    int x, y;
};

// 原发器 - 象棋棋子
class Chessman
{
public:
    Chessman(string l, int xx, int yy) : label(l), x(xx), y(yy) {}
    void setLabel(string l) { label = l; }
    void setX(int xx) { x = xx; }
    void setY(int yy) { y = yy; }
    string getLabel() { return label; }
    int getX() { return x; }
    int getY() { return y; }
    ChessmanMemento save() { return ChessmanMemento(label, x, y); }
    void restore(ChessmanMemento memento) 
    { 
        label = memento.getLabel();
        x = memento.getX();
        y = memento.getY();
    }
    void display() { cout << label << ": (" << x << ", " << y << ")" << endl; }

private:
    string label;
    int x, y;
};

// 负责人类 - 备忘录管理类
class MementoCaretaker
{
public:
    ChessmanMemento getMemento() { return memento; }
    void setMemento(ChessmanMemento mem) { memento = mem; }

private:
    ChessmanMemento memento;
};

int main(int argc, const char* argv[])
{
    MementoCaretaker mc;
    Chessman cm("车", 8, 7);
    cm.display();
    mc.setMemento(cm.save());
    cm.setX(12);
    cm.setY(19);
    cm.display();
    cm.restore(mc.getMemento());
    cm.display();

    return 0;
}
