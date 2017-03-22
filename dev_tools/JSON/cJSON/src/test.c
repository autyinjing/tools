/* ***********************************************************************

  > File Name: test.c
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年03月22日 星期三 10时24分17秒

 ********************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"

int main(int argc, const char* argv[])
{
    char *text = "{\"name\": \"Jack (\\\"Bee\\\") Nimble\", "
                "\"format\":{\"type\": \"rect\", "
                "\"width\": 1920, \"interlace\": false}}";

    cJSON *root = cJSON_Parse(text);
    if (!root)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return EXIT_FAILURE;
    }

    char *out = cJSON_Print(root);
    printf("text:\n%s\n\n", out);
    free(out);

    char *name = cJSON_GetObjectItem(root, "name")->valuestring;
    printf("name: %s\n", name);
    cJSON *format = cJSON_GetObjectItem(root, "format");
    int width = cJSON_GetObjectItem(format, "width")->valueint;
    printf("%s->width: %d\n", format->string, width);

    cJSON_Delete(root);

    return EXIT_SUCCESS;
}

