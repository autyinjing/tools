/* ***********************************************************************

  > File Name: prime_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月17日 星期三 18时28分46秒

 ********************************************************************** */

#include <iostream>
#include "prime.h"

int main(int argc, const char* argv[])
{
    getPrimeTable();
    for (uint32_t i = 0; i < 100; ++i)
    {
        std::cout << primes[i] << " ";
        if (!((i+1) % 10))
            std::cout << std::endl;
    }

    return 0;
}
