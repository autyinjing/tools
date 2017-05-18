/* ***********************************************************************

  > File Name: strategy_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月18日 星期四 10时37分30秒

 ********************************************************************** */

// 策略模式
#include <iostream>

// 抽象策略类 - 飞机
class PlaneBase
{
public:
    virtual ~PlaneBase() {}
    virtual void fly() = 0;
};

// 环境类 - 模拟系统
class SimulationSystem
{
public:
    SimulationSystem() : m_plane(nullptr) {}
    void fly() { if (m_plane != nullptr) m_plane->fly(); }
    void setPlane(PlaneBase *plane) { m_plane = plane; }

private:
    PlaneBase *m_plane;
};

// 具体策略类 - 直升机
class Helicopter : public PlaneBase
{
public:
    void fly() { std::cout << "Helicopter: 垂直起飞，亚音速飞行" << std::endl; }
};

// 具体策略类 - 客机
class AirPlane : public PlaneBase
{
public:
    void fly() { std::cout << "AirPlane: 长距离起飞，亚音速飞行" << std::endl; }
};

// 具体策略类 - 歼击机
class Fighter : public PlaneBase
{
public:
    void fly() { std::cout << "Fighter: 长距离起飞，超音速飞行" << std::endl; }
};

// 具体策略类 - 鹞式战斗机
class Harrier : public PlaneBase
{
public:
    void fly() { std::cout << "Harrier: 垂直起飞，超音速飞行" << std::endl; }
};

#include <iostream>

int main(int argc, const char* argv[])
{
    SimulationSystem ss;
    PlaneBase *pb1 = new Helicopter;
    PlaneBase *pb2 = new AirPlane;
    PlaneBase *pb3 = new Fighter;
    PlaneBase *pb4 = new Harrier;

    ss.fly();
    ss.setPlane(pb1);
    ss.fly();
    ss.setPlane(pb2);
    ss.fly();
    ss.setPlane(pb3);
    ss.fly();
    ss.setPlane(pb4);
    ss.fly();

    delete pb1;
    delete pb2;
    delete pb3;
    delete pb4;

    return 0;
}
