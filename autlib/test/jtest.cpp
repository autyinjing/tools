/* ***********************************************************************

  > File Name: jtest.cpp
  > Author: Aut
  > Mail: autyinjing@126.com 
  > Created Time: 2018年08月09日 星期四 17时53分49秒

 ********************************************************************** */

#include <iostream>
#include "../c++/json/JsonUtil.h"

int main(int argc, const char* argv[])
{
    JsonUtil json;
    json.addString("key", "Aut");
    std::cout << json.toNowNodeString() << std::endl;

    return 0;
}
