/* ***********************************************************************

  > File Name: cmd_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月23日 星期二 11时04分33秒

 ********************************************************************** */

// 命令模式

#include <iostream>
#include <string>
#include <vector>

// 抽象命令类
class CmdBase
{
public:
    virtual ~CmdBase() {}
    virtual void execute() = 0;
};

// 调用者 - 菜单项
class MenuItem
{
public:
    MenuItem() : m_cmd(nullptr) {}
    MenuItem(CmdBase *cmd) : m_cmd(cmd) {}
    void click() { if (m_cmd != nullptr) m_cmd->execute(); }
    void setCmd(CmdBase *cmd) { m_cmd = cmd; }

private:
    CmdBase *m_cmd;
};

// 业务类 - 主菜单
class Menu
{
public:
    void addMenuItem(MenuItem *mi) { m_mi.push_back(mi); }
    void onClick(uint32_t menuId) { if (menuId < m_mi.size()) m_mi[menuId]->click(); }

private:
    std::vector<MenuItem *> m_mi;
};

// 接收者 - 系统界面类
class BoardScreen
{
public:
    void open() { std::cout << "BoardScreen::open()\n"; }
    void create() { std::cout << "BoardScreen::create()\n"; }
    void edit() { std::cout << "BoardScreen::edit()\n"; }
};

// 具体命令类 - 打开
class OpenCmd : public CmdBase
{
public:
    void execute() { m_bs.open(); }

private:
    BoardScreen m_bs;
};

// 具体命令类 - 创建
class CreateCmd : public CmdBase
{
public:
    void execute() { m_bs.create(); }

private:
    BoardScreen m_bs;
};

// 具体命令类 - 编辑
class EditCmd : public CmdBase
{
public:
    void execute() { m_bs.edit(); }

private:
    BoardScreen m_bs;
};

int main(int argc, const char* argv[])
{
    CmdBase *oc = new OpenCmd;
    CmdBase *cc = new CreateCmd;
    CmdBase *ec = new EditCmd;
    MenuItem *mi1 = new MenuItem(oc);
    MenuItem *mi2 = new MenuItem;
    mi2->setCmd(cc);
    MenuItem *mi3 = new MenuItem(ec);

    Menu mainMenu;
    mainMenu.addMenuItem(mi1);
    mainMenu.addMenuItem(mi2);
    mainMenu.addMenuItem(mi3);
    mainMenu.onClick(1);
    for (uint32_t i = 0; i < 3; ++i)
        mainMenu.onClick(i);

    delete mi1;
    delete mi2;
    delete mi3;
    delete oc;
    delete cc;
    delete ec;

    return 0;
}
