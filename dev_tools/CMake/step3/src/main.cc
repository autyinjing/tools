/* ***********************************************************************

  > File Name: main.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年03月09日 星期四 10时00分12秒

 ********************************************************************** */

#include <iostream>
#include "../cache/define/test.pb.h"

int main(int argc, const char* argv[])
{
    test::test t;
    std::cout << t.name() << std::endl;

    return 0;
}

