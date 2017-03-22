
### 下载

地址：++http://sourceforge.net/projects/cjson/++

### 数据结构

```
typedef struct cJSON {
    struct cJSON *next,*prev;   /* 指向兄弟结点 */
    struct cJSON *child;        /* 指向子结点 */
    int type;                   /* value类型 */

    /* 保存value */
    char *valuestring;
    int valueint;
    double valuedouble;

    /* 保存key */
    char *string;
} cJSON;
```

### 常用API

```
cJSON_CreateObject();   /* 创建结点 */
cJSON_AddxxxToObject(); /* 添加子结点 */
cJSON_Print();          /* 返回字符串结果 */
cJSON_Delete();         /* 释放结点 */

cJSON_Parse();          /* 从字符串解析 */
cJSON_GetObjectItem();  /* 获取子结点 */
```

### 实例

构造cJSON对象：
```
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
```

从字符串解析：
```
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
```

### 注意点

    - cJSON_CreateObject()和cJSON_Print()会申请内存空间，需要使用cJSON_Delete()和free()手动释放；
    - 可以使用cJSON_GetErrorPtr()获取错误信息；
    - 可以使用cJSON_InitHooks()定制malloc和free，以便检测内存的使用情况；
    