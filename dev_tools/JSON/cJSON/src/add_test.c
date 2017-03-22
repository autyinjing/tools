/* ***********************************************************************

  > File Name: add_test.c
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年03月22日 星期三 16时32分38秒

 ********************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"

int main(int argc, const char* argv[])
{
    cJSON *root, *fmt;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "Jack (\"Bee\") Nimble");
    fmt = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "format", fmt);
    cJSON_AddStringToObject(fmt, "type", "rect");
    cJSON_AddNumberToObject(fmt, "width", 1920);
    cJSON_AddFalseToObject(fmt, "interlace");

    char *result = cJSON_Print(root);
    puts(result);

    free(result);
    cJSON_Delete(root);

    return EXIT_SUCCESS;
}

