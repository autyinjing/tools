/* ***********************************************************************

  > File Name: oftest.c
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年04月25日 星期二 10时45分04秒

 ********************************************************************** */

#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER);

struct str {
    double  b;
    int     a;
    char    c;
};

int main(int argc, const char* argv[])
{
    long int off = offsetof(typeof(struct str), c);
    printf("offset: %ld\n", off);

    return 0;
}

