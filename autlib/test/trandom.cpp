/* ***********************************************************************

  > File Name: trandom.cpp
  > Author: Aut
  > Mail: autyinjing@126.com 
  > Created Time: 2018年08月09日 星期四 21时24分07秒

 ********************************************************************** */

#include <iostream>
#include "../c++/random/RandomGenerator.h"

int main(int argc, const char* argv[])
{
    AutLib::RandomGenerator random_generator;

    std::cout << random_generator.RandomInt(87, 1219) << std::endl
              << random_generator.RandomRange(9.11, 12.11) << std::endl
              << random_generator.NextDouble() << std::endl;

    return 0;
}
