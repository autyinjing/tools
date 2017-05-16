/* ***********************************************************************

  > File Name: flyweight_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月16日 星期二 15时31分25秒

 ********************************************************************** */

// 享元模式 - 单纯享元模式

#include <iostream>
#include <string>
#include <map>
#include "../../c++/design_patterns/singleton.hpp"

// 抽象享元类 - 多媒体资料
enum MmdType {
    IMAGE       = 1,
    VIDEO       = 2,
    ANIMATION   = 3,
    MAX_TYPE
};

struct MmdInfo {
    std::string name;
    uint32_t    type;
    MmdInfo(std::string n, uint32_t t) : name(n), type(t) {}
    MmdInfo(const MmdInfo &mmd) { name = mmd.name; type = mmd.type; }
    friend bool operator<(const MmdInfo &lhr, const MmdInfo &rhr)
    {
        return ((lhr.name != rhr.name) || (lhr.type != rhr.type));
    }
};

struct ExteraInfo {
    uint32_t x;
    uint32_t y;
    uint32_t size;
    ExteraInfo(uint32_t xx, uint32_t yy, uint32_t s) : x(xx), y(yy), size(s) {}
};

class MultiMediaData
{
public:
    MultiMediaData(const MmdInfo &mmd) : m_mmd(mmd) {}
    virtual ~MultiMediaData() {}
    virtual void display(const ExteraInfo &ei) = 0;

protected:
    MmdInfo m_mmd;
};

// 具体享元类 - 图片
class Image : public MultiMediaData
{
public:
    Image(const MmdInfo &mmd) : MultiMediaData(mmd) {}
    void display(const ExteraInfo &ei)
    {
        std::cout << "MemAddr: "  << this << " " << m_mmd.name << "(IMAGE): " 
                  << "(" << ei.x << ", " << ei.y << ", " << ei.size << ")" << std::endl;
    }
};

// 具体享元类 - 视频
class Video : public MultiMediaData
{
public:
    Video(const MmdInfo &mmd) : MultiMediaData(mmd) {}
    void display(const ExteraInfo &ei)
    {
        std::cout << "MemAddr: "  << this << " " << m_mmd.name << "(VIDEO): " 
                  << "(" << ei.x << ", " << ei.y << ", " << ei.size << ")" << std::endl;
    }
};

// 具体享元类 - 动画
class Animation : public MultiMediaData
{
public:
    Animation(const MmdInfo &mmd) : MultiMediaData(mmd) {}
    void display(const ExteraInfo &ei)
    {
        std::cout << "MemAddr: "  << this << " " << m_mmd.name << "(ANIMATION): " 
                  << "(" << ei.x << ", " << ei.y << ", " << ei.size << ")" << std::endl;
    }
};

// 享元工厂类
class FlyweightFactory : public Singleton<FlyweightFactory>
{
public:
    MultiMediaData *getMmd(const MmdInfo &mmd)
    {
        MultiMediaData *newMmd = nullptr;
        std::map<MmdInfo, MultiMediaData *>::iterator it = m_mmd.find(mmd);
        if (it != m_mmd.end()) {
            newMmd = it->second;
        } else {
            switch (mmd.type)
            {
            case IMAGE:
                newMmd = new Image(mmd);
                break;
            case VIDEO:
                newMmd = new Video(mmd);
                break;
            case ANIMATION:
                newMmd = new Animation(mmd);
                break;
            default:
                return nullptr;
            }
            m_mmd.insert(std::make_pair(mmd, newMmd));
        }
        return newMmd;
    }
    void destory()
    {
        std::map<MmdInfo, MultiMediaData *>::iterator it = m_mmd.begin();
        for (; it != m_mmd.end(); ++it) {
            delete it->second;
            it->second = nullptr;
        }
        m_mmd.clear();
    }

private:
    std::map<MmdInfo, MultiMediaData *> m_mmd;
};

int main(int argc, const char* argv[])
{
    FlyweightFactory *pFf  = FlyweightFactory::get_instance_ptr();
    MultiMediaData *pImg1  = pFf->getMmd(MmdInfo("a.jpg", IMAGE));
    MultiMediaData *pImg2  = pFf->getMmd(MmdInfo("a.jpg", IMAGE));
    MultiMediaData *pImg3  = pFf->getMmd(MmdInfo("a.jpg", IMAGE));
    MultiMediaData *pVideo = pFf->getMmd(MmdInfo("b.mkv", VIDEO));
    MultiMediaData *pAni   = pFf->getMmd(MmdInfo("c.mp4", ANIMATION));

    pImg1->display(ExteraInfo(1,2,33));
    pImg2->display(ExteraInfo(4,5,66));
    pImg3->display(ExteraInfo(7,8,99));
    pVideo->display(ExteraInfo(10,11,22));
    pAni->display(ExteraInfo(13,14,55));

    pFf->destory();

    return 0;
}
