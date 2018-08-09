/* ***********************************************************************

  > File Name: chrono.cpp
  > Author: Aut
  > Mail: autyinjing@126.com 
  > Created Time: 2018年08月09日 星期四 21时09分19秒

 ********************************************************************** */

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

int main(int argc, const char* argv[])
{
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    std::cout << (unsigned int)t1.time_since_epoch().count() << std::endl;

    return 0;
}
