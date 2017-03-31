#ifndef COMMON_FUNCTION_HPP
#define COMMON_FUNCTION_HPP
#include<time.h>
#include<iostream>

inline struct tm* g_get_struct_time(uint32_t time_value){
	time_t m_time = time_value;
	struct tm* data;
	data = localtime(&m_time);
	return data;
}

inline uint32_t g_get_strtotime(const char* str)
{           
    struct tm* dt = localtime(&get_now_tv()->tv_sec);
    uint32_t year,mon,day,hour,min,sec;
    sscanf(str,"%u-%u-%u %u:%u:%u", &year, &mon, &day, &hour, &min, &sec);
    dt->tm_year = year - 1900;
    dt->tm_mon = mon - 1;
    dt->tm_mday = day;
    dt->tm_hour = hour;
    dt->tm_min = min;
    dt->tm_sec = sec;
    return mktime(dt);
}

inline uint32_t g_globle_end_of_day()
{
	struct tm* dt;
	time_t new_time = get_now_tv()->tv_sec + 24*3600;
	dt = localtime(&new_time);

	dt->tm_hour = 0;
	dt->tm_min = 0;
	dt->tm_sec = 0;
	return mktime(dt);
}

inline uint32_t g_globle_end_of_week()
{
	struct tm* dt;
	time_t now_time = get_now_tv()->tv_sec;
	dt = localtime(&now_time);

	dt->tm_wday = 6;
	dt->tm_hour = 23;
	dt->tm_min = 59;
	dt->tm_sec = 59;
	return mktime(dt);
}

inline uint32_t g_globle_end_of_month()
{
	struct tm* data;
	time_t month_end;
	time_t now_time = get_now_tv()->tv_sec;
	data = localtime(&now_time);

	if(data->tm_mon == 1){
		bool is_leap = true;
		uint32_t year = data->tm_year + 1900;

		if(year%4){
			is_leap = false;
		}else{
			if(year%100 == 0 && year%400 != 0){
				is_leap = false;
			}
		}
		if(is_leap){
			data->tm_mday = 29;
		}else{
			data->tm_mday = 28;
		}
	}else{
		if(data->tm_mon < 7){
			if(data->tm_mon%2){
				data->tm_mday = 30;
			}else{
				data->tm_mday = 31;
			}
		}else{
			if(data->tm_mon%2){
				data->tm_mday = 31;
			}else{
				data->tm_mday = 30;
			}
		}
	}
	data->tm_hour = 23;
	data->tm_min = 59;
	data->tm_sec = 59;
	month_end = mktime(data);
	return month_end;
}

inline uint32_t g_globle_begin_of_month()
{
	struct tm* data;
	time_t month_begin;
	time_t now_time = get_now_tv()->tv_sec;
	data = localtime(&now_time);

	data->tm_mday = 1;
	data->tm_hour = 0;
	data->tm_min = 0;
	data->tm_sec = 0;
	month_begin = mktime(data);
	return month_begin;
}

/* @brief 获取系统时间对应的年，月，日
   @in:year, month；day(传进的是定义好的变量)
   @out:year, month；day（传出的是通过计算得出系统时间的年， 月，日）
        eg: 如今天是2014年5月19日；则year:2014; month:5; day:19
 */
inline void g_globle_get_ymd_info(uint32_t& year, uint32_t& month, uint32_t& day)
{
	time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year + 1900;
   	month = timeinfo->tm_mon + 1;
    day = timeinfo->tm_mday;
}

/** @brief 计算当日零点时间
 */
inline uint32_t xhx_day_start_time()
{
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    rawtime = mktime(timeinfo);
    return rawtime;
}

/* @brief 获取当天日期
 * @return 格式为"YYYYMMDD"
 */
inline int get_date_yyyymmdd(time_t rawtime)
{
    struct tm * timeinfo = localtime(&rawtime);
    return (timeinfo->tm_year+1900)*10000+(timeinfo->tm_mon+1)*100+timeinfo->tm_mday;
}

/* @brief 判断时间是否是今天
 * @param time_seconds 要判断的时间
 * @return true 是今天，false 不是今天
 */
inline bool time_is_today(uint32_t time_seconds)
{
    return (get_date_yyyymmdd(time(0)) == get_date_yyyymmdd(time_seconds));
}

/* @brief 压缩存储，一个uint32_t存储多个数值，设置某个bit区间为data
 * @param   old_value：原数据
 *          id：区间序号
 *          data：要设置的值
 *          bit_num：区间长度
 * @注意：bit_num只能取2的整数次幂，可取的值为(1,2,4,8,16,32)
 */
inline bool set_uint32_byte_value(uint32_t& old_value, uint32_t id, uint32_t data, uint32_t bit_num)
{
    if (bit_num > 32 || id < 1) {
        //参数非法
        return false;
    }
    if (data > (uint32_t)((1 << bit_num) - 1)) {
        //数值过大
        return false;
    }

    uint32_t offset = (id - 1);
    uint32_t tmp_1 = ((1<<bit_num)-1) << (offset*bit_num);
    uint32_t tmp_2 = (~tmp_1) | (data << (offset*bit_num));
    //将对应位置全置1
    old_value |= tmp_1;
    //将新值放进去
    old_value &= tmp_2;

    return true;
}

/*
 * @brief 获取uint32_t某个bit区间的值，对应上一个函数
 */
inline uint32_t get_badge_byte_value(uint32_t old_value, uint32_t id, uint32_t bit_num)
{
    if (bit_num > 32 || id < 1) {
        return 0;
    }
    uint32_t value_cnt = 32 / bit_num, offset = id - 1;
    if (id > value_cnt)
        return 0;

    uint32_t help = old_value;
    //获取该值
    help = (help << ((value_cnt-(offset+1))*bit_num)) >> (((value_cnt-(offset+1))*bit_num)+offset*bit_num);

    return help;
}

/*
 * @brief 设置uint32_t某个bit为1
 */
inline bool set_uint32_bit_flag(uint32_t& old_value, uint32_t id)
{
    if (id < 1 || id > 32)
        return false;

    old_value |= (1 << (id - 1));

    return true;
}

/*
 * @brief 设置uint32_t某个bit为0
 */
inline bool reset_uint32_bit_flag(uint32_t& old_value, uint32_t id)
{
    if (id < 1 || id > 32)
        return false;

    uint32_t help = (1 << (id - 1));
    help = ~help;
    old_value &= help;

    return true;
}

/*
 * @brief 获取uint32_t某个区间内值为1的bit数量
 */
inline uint32_t get_uint32_bit_set_cnt(uint32_t value, uint32_t begin, uint32_t end)
{
    if (begin < 1 || end > 32 || begin > end)
        return 0;

    uint32_t cnt = 0, mask = (1 << (begin - 1));
    while (begin <= end)
    {
        if (value & mask)
            ++cnt;
        mask <<= 1;
        ++begin;
    }

    return cnt;
}

#endif
