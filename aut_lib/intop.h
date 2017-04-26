#ifndef AUT_INTOP_H
#define AUT_INTOP_H

#ifdef __cplusplus
extern "C" {
#endif  //end of __cplusplus

/* @brief 压缩存储，一个uint32_t存储多个数值，设置某个bit区间为data
 * @param   old_value： 原数据
 *          id：        区间序号
 *          data：      要设置的值
 *          bit_num：   区间长度
 * @注意：bit_num只能取2的整数次幂，可取的值为(1,2,4,8,16,32)
 */
static inline bool set_uint32_byte_value(uint32_t& old_value, uint32_t id, uint32_t data, uint32_t bit_num)
{
    if (bit_num > 32 || id < 1)
        return false;
    if (data > static_cast<uint32_t>((1 << bit_num) - 1))
        return false;

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
static inline uint32_t get_badge_byte_value(uint32_t old_value, uint32_t id, uint32_t bit_num)
{
    if (bit_num > 32 || id < 1)
        return 0;
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
static inline bool set_uint32_bit_flag(uint32_t& old_value, uint32_t id)
{
    if (id < 1 || id > 32)
        return false;

    old_value |= (1 << (id - 1));

    return true;
}

/*
 * @brief 设置uint32_t某个bit为0
 */
static inline bool reset_uint32_bit_flag(uint32_t& old_value, uint32_t id)
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
static inline uint32_t get_uint32_bit_set_cnt(uint32_t value, uint32_t begin, uint32_t end)
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

#ifdef __cplusplus
}
#endif  //end of __cplusplus

#endif  //end of AUT_INTOP_H
