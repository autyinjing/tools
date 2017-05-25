/* ***********************************************************************

  > File Name: state_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月25日 星期四 11时51分06秒

 ********************************************************************** */

// 状态模式

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

// 抽象状态类 - 用户等级
class LevelBase
{
public:
    virtual ~LevelBase() {}
    virtual void play() {}
    virtual void doubleScore() {}
    virtual void changeCards() {}
    virtual void peekCards() {}
};

// 具体状态类 - 入门级
class Primary : public LevelBase
{
public:
    void play() { cout << "I can play()" << endl; }
};

// 具体状态类 - 熟练级
class Secondary : public LevelBase
{
public:
    void play() { cout << "I can play()" << endl; }
    void doubleScore() { cout << "I can doubleScore()" << endl; }
};

// 具体状态类 - 高手级
class Professional : public LevelBase
{
public:
    void play() { cout << "I can play()" << endl; }
    void doubleScore() { cout << "I can doubleScore()" << endl; }
    void changeCards() { cout << "I can changeCards()" << endl; }
};

// 具体状态类 - 骨灰级
class Final : public LevelBase
{
public:
    void play() { cout << "I can play()" << endl; }
    void doubleScore() { cout << "I can doubleScore()" << endl; }
    void changeCards() { cout << "I can changeCards()" << endl; }
    void peekCards() { cout << "I can peekCards()" << endl; }
};

// 环境类 - 角色
class Player
{
public:
    Player(string n) : name(n), score(0), level(nullptr) {}
    ~Player() { if (level != nullptr) { delete level; level = nullptr; } }
    void changeScore(int s)
    {
        cout << "Old score: " << score << ", Chenged score: " << s << endl;
        score += s;
        if (level != nullptr) {
            delete level;
            level = nullptr;
        }
        if (score < 10)
            level = new Primary;
        else if (score < 100)
            level = new Secondary;
        else if (score < 1000)
            level = new Professional;
        else
            level = new Final;
    }
    void display()
    {
        cout << "name: " << name << ", score: " << score << endl;
        if (level != nullptr) {
            level->play();
            level->doubleScore();
            level->changeCards();
            level->peekCards();
        }
    }

private:
    string  name;
    int     score;
    LevelBase *level;
};

int main(int argc, const char* argv[])
{
    Player player("aut");
    player.changeScore(10000);
    player.display();
    player.changeScore(-9200);
    player.display();
    player.changeScore(-780);
    player.display();
    player.changeScore(-11);
    player.display();

    return 0;
}
