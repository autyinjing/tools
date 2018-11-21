/* ***********************************************************************

  > File Name: LRU.h
  > Author: Aut
  > Mail: autyinjing@126.com 
  > Created Time: 2018年10月11日 星期四 17时48分53秒

 ********************************************************************** */

#ifndef AUTLIB_LRU_H
#define AUTLIB_LRU_H

#include <map>
#include <list>

template<typename K, typename V>
class LRU
{
private:
    using list_it = std::list<std::pair<K, V>>::iterator;
    using map_it  = std::map<K, list_it>::iterator;
    std::list<std::pair<K, V>> l_;  //存储数据
    std::map<K, list_it> m_;        //存储数据索引

public:
    LRU() {}
    virtual ~LRU() {}

    //插入到队尾
    //删除队首
    //获取队首
    //删除指定元素
    //是否包含
    //是否为空
    //元素数量
    //移动元素到队尾
};

#endif  //AUTLIB_LRU_H
