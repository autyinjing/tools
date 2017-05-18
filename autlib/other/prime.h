/* ***********************************************************************

  > File Name: prime.h
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月11日 星期四 18时30分45秒

 ********************************************************************** */

#ifndef AUT_PRIME_H
#define AUT_PRIME_H

#ifdef __cplusplus
extern "C" {
#endif

const uint32_t M = 30000;
const uint32_t N = 10000;
bool check[M] = {true, true};
uint32_t primes[N];
uint32_t prime_cnt = 0;

void getPrimeTable()
{
    for (uint32_t i = 2; i < M; ++i)
    {
        if (!check[i])
            primes[prime_cnt++] = i;
        for (uint32_t j = 0; (j < prime_cnt) && (i*primes[j] < M); ++j)
        {
            check[i*primes[j]] = true;
            if (!(i % primes[j]))
                break;
        }
    }
}

#ifdef __cplusplus
}
#endif  // end of __cplusplus

#endif  // end of AUT_PRIME_H
