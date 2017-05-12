/* ***********************************************************************

  > File Name: combination_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月11日 星期四 16时23分02秒

 ********************************************************************** */

// 组合模式，使用透明组合模式

#include <iostream>
#include <vector>
#include <string>

class Widget
{
public:
    Widget() { m_name = ""; }
    Widget(std::string name) { m_name = name; }
    virtual ~Widget() {}

    virtual void show() { std::cout << "Widge::show()" << std::endl;  }
    virtual void hide() { std::cout << "Widget::hide()" << std::endl;  }
    virtual void add(Widget *w) { std::cout << "Widget::add()" << std::endl; }
    virtual void remove(Widget *w) { std::cout << "Widget::remove()" << std::endl;  }
    virtual std::string getName() { return m_name; }

protected:
    std::string m_name;
};

class Button : public Widget
{
public:
    Button(std::string name) : Widget(name) {}
    void show() { std::cout << m_name << " - Button::show()" << std::endl;  }
    void hide() { std::cout << m_name << " - Button::hide()" << std::endl;  }
};

class TextBox : public Widget
{
public:
    TextBox(std::string name) : Widget(name) {}
    void show() { std::cout << m_name << " - TextBox::show()" << std::endl;  }
    void hide() { std::cout << m_name << " - TextBox::hide()" << std::endl;  }
};

class Form : public Widget
{
public:
    Form(std::string name) : Widget(name) {}
    void show() 
    {
        std::vector<Widget *>::iterator it = m_list.begin();
        for (; it != m_list.end(); ++it)
            (*it)->show();
    }
    void hide()
    {
        std::vector<Widget *>::iterator it = m_list.begin();
        for (; it != m_list.end(); ++it)
            (*it)->hide();
    }
    void add(Widget *w) 
    { 
        if (w != nullptr) {
            m_list.push_back(w); 
            std::cout << m_name <<  " - For::add() " << w->getName() << std::endl;
        }
    }
    void remove(Widget *w) 
    { 
        if (w == nullptr)
            return ;
        std::vector<Widget *>::iterator it = m_list.begin();
        for (; it != m_list.end(); ++it)
            if ((*it) == w)
                break;
        if (it != m_list.end())
            m_list.erase(it);
        std::cout << m_name << " - Form::remove() " << w->getName() << std::endl;
    }

private:
    std::vector<Widget *> m_list;
};

int main()
{
    Widget *mainUI  =   new Form("main");
    Widget *textBox =   new TextBox("tb");
    Widget *button  =   new Button("bt");

    textBox->add(mainUI);
    button->remove(textBox);
    button->show();
    mainUI->add(textBox);
    mainUI->add(button);
    mainUI->show();
    mainUI->hide();
    mainUI->remove(button);
    mainUI->show();

    delete mainUI;
    delete textBox;
    delete button;

    return 0;
}
