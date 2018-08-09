/* ***********************************************************************

  > File Name: Util.h
  > Author: Aut
  > Mail: autyinjing@126.com 
  > Created Time: 2018年07月19日 星期四 19时04分47秒

 ********************************************************************** */

#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <stdint.h>

namespace AutLib { namespace Util {

    enum enmTimeSecond
    {
        OneMinuteSecond     = 60,       //一分钟的秒数
        OneHourSecond       = 3600,     //一小时的秒数
        OneDaySecond        = 86400,    //一天的秒数
        OneWeekSecond       = 6048000,  //一周的秒数

        TimeValueEnd
    }

    //获取今天hh:mm:ss的秒数
    static inline uint32_t today_sometime_second(uint32_t hh = 0, uint32_t mm = 0, uint32_t ss = 0)
    {
        time_t rawtime;
        time(&rawtime);
        struct tm * timeinfo = localtime(&rawtime);
        timeinfo->tm_hour   = hh;
        timeinfo->tm_min    = mm;
        timeinfo->tm_sec    = ss;
        rawtime = mktime(timeinfo);
        return rawtime;
    }

    //获取今天00:00:00的秒数
    static inline uint32_t today_start_second()
    {
        return today_sometime_second();
    }

    //获取明天00:00:00的秒数
    static inline uint32_t tomorrow_start_second()
    {
        time_t rawtime;
        time(&rawtime);
        struct tm * timeinfo = localtime(&rawtime);
        timeinfo->tm_mday   += 1;
        timeinfo->tm_hour   = 0;
        timeinfo->tm_min    = 0;
        timeinfo->tm_sec    = 0;
        rawtime = mktime(timeinfo);
        return rawtime;
    }

    //设置uint32_t的某一bit为1
    static inline void set_uint32_bit_flag(uint32_t &val, uint32_t bit)
    {
        if (bit < 1 || bit > 30)
            return ;
        val |= (1 << (bit - 1));
    }

    //设置uint32_t的区间[beg,end]之间的bit为1
    static inline void set_uint32_bit_flag(uint32_t &val, uint32_t beg, uint32_t end)
    {
        if (beg > end || beg < 1 || end > 30)
            return ;
        uint32_t mask = ((1 << end) - 1) ^ ((1 << (beg - 1)) - 1);
        val |= mask;
    }

    //判断uint32_t的某一bit是否为1
    static inline bool test_uint32_bit_flag(uint32_t val, uint32_t bit)
    {
        if (bit < 1 || bit > 30)
            return false;
        return val & (1 << (bit - 1));
    }

    //计算uint32_t的区间[beg,end]之间bit为1的数量
    static inline uint32_t get_uint32_bit_flag_num(uint32_t val, uint32_t beg, uint32_t end)
    {
        if (beg > end || beg < 1 || end > 30)
            return 0;
        uint32_t num = 0;
        for (uint32_t i = beg - 1; i < end; ++i)
        {
            if (val & (1 << i))
                ++num;
        }
        return num;
    }

}   //end of Util
}   //end of AutLib
#endif      //end of UTIL_H
