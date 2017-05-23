/* ***********************************************************************

  > File Name: interpret_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月23日 星期二 14时56分11秒

 ********************************************************************** */

// 解释器模式

#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::istringstream;

// 抽象表达式
class NodeBase
{
public:
    virtual ~NodeBase() {}
    virtual string interpret() = 0;
};

// 非终结符表达式 - 拷贝
class CopyNode : public NodeBase
{
public:
    CopyNode(NodeBase *obj, NodeBase *src, NodeBase *des)
        : m_obj(obj), m_src(src), m_des(des) {}
    string interpret() 
    { 
        cout << "COPY " << m_obj->interpret()
             << "FROM " << m_src->interpret()
             << "TO "   << m_des->interpret()
             << endl;
    }

private:
    NodeBase *m_obj;
    NodeBase *m_src;
    NodeBase *m_des;
};

// 非终结符表达式 - 移动
class MoveNode : public NodeBase
{
public:
    MoveNode(NodeBase *obj, NodeBase *src, NodeBase *des)
        : m_obj(obj), m_src(src), m_des(des) {}
    string interpret() 
    { 
        cout << "MOVE " << m_obj->interpret()
             << "FROM " << m_src->interpret()
             << "TO "   << m_des->interpret()
             << endl;
    }

private:
    NodeBase *m_obj;
    NodeBase *m_src;
    NodeBase *m_des;
};

// 终结符表达式 - 视图
class ViewNode : public NodeBase
{
public:
    ViewNode(string name) : m_name(name) {}
    string interpret() { cout << "VIEW "; if (m_name != "") cout << m_name; }

private:
    string m_name;
};

// 终结符表达式 - 表
class TableNode : public NodeBase
{
public:
    TableNode(string name) : m_name(name) {}
    string interpret() { cout << "TABLE " << m_name; }

private:
    string m_name;
};

// 环境类 - 文法处理器
class Handler
{
public:
    Handler(string sentence) : m_sen(sentence), m_node(nullptr) {}
    void handle()
    {
        istringstream input(m_sen);
        string op, obj, obj_name, src, des, help;
        input >> op >> obj >> obj_name;
        if (obj == "TABLE" && obj_name == "FROM")
            return;
        if (obj_name == "FROM") {
            input >> src >> help >> des;
        } else {
            input >> help >> src >> help >> des;
        }
        if (op == "COPY") {
            m_node = new CopyNode();
        } else if (op == "MOVE") {
            m_node = new MoveNode();
        }
    }
    void interpret() { if (m_node != nullptr) m_node->interpret(); }

private:
    string   m_sen;
    NodeBase *m_node;
};

int main(int argc, const char* argv[])
{

    return 0;
}
