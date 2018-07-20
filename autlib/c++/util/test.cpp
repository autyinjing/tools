/* ***********************************************************************

  > File Name: test.cpp
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2018年07月20日 星期五 14时36分47秒

 ********************************************************************** */

#include <iostream>
#include "Util.h"

using namespace std;
using namespace AutLib::Util;

int main(int argc, const char* argv[])
{
    uint32_t val = 1023;
    set_uint32_bit_flag(val, 11, 21);
    //cout << test_uint32_bit_flag(val, 21) << " " << val << endl;
    cout << get_uint32_bit_flag_num(val, 1, 30) << endl;

    return 0;
}

