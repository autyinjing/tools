/* ***********************************************************************

  > File Name: asm.c
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年04月25日 星期二 11时59分33秒

 ********************************************************************** */

#include <stdio.h>

#define barrier() __asm__ __volatile__("": : :"memory")

int main(int argc, const char* argv[])
{
    int a = __WORDSIZE;
    printf("%d\n", a);

    return 0;
}

