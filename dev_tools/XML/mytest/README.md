
### 下载

地址：http://xmlsoft.org/downloads.html

### 关键数据结构

```
typedef struct _xmlNode xmlNode;
typedef xmlNode *xmlNodePtr;
struct _xmlNode {   /* 只列出几个比较重要的 */
    xmlElementType   type;      /* 节点类型 */
    const xmlChar   *name;      /* 结点名称 */
    struct _xmlNode *parent;    /* 父节点指针 */
    struct _xmlNode *children;  /* 子结点链表 */
    struct _xmlNode *last;      /* 最后一个子节点 */
    struct _xmlNode *prev;      /* 上一个兄弟节点  */
    struct _xmlNode *next;      /* 下一个兄弟节点  */

    /* ... */

    xmlChar         *content;   /* 节点内容 */
    struct _xmlAttr *properties;/* 属性列表 */
};
```

### 常用API

```
/* 创建xml文档 */
xmlNewDoc()             /* 创建文档 */
xmlDocSetRootElement()  /* 设置根节点 */
xmlNewNode()            /* 创建节点 */
xmlNewProp()            /* 添加属性 */
xmlNewChild()           /* 创建最底层子节点 - 子节点没有孙子节点 */
xmlAddChild()           /* 添加子节点 - 子节点还有孙子节点*/

/* 从xml文档解析 */
xmlGetProp()            /* 获取节点属性 */
xmlNodeListGetString()  /* 获取节点内容 */

/* 文件操作 */
xmlSaveFormatFileEnc()  /* 将文档保存成文件 - 提供额外选项 */
xmlSaveFile()           /* 将文档保存成文件 */
xmlParseFile()          /* 从文件解析 */
xmlReadFile()           /* 从文件解析 - 文件名可以是URL，并且提供额外选项 */

/* 其他 */
xmlFreeDoc()            /* 释放文档使用的内存 */
xmlStrcmp()             /* 字符串比较 */
```

### 实例

注：为示例简洁，下面的代码省略了错误处理相关的代码
##### 创建xml文档
```
#include <stdio.h>

#include <libxml/tree.h>

int main(int argc, const char* argv[])
{
    xmlDocPtr doc = NULL;
    doc = xmlNewDoc(BAD_CAST"1.0");

    //创建根节点
    xmlNodePtr root = xmlNewNode(NULL, BAD_CAST"phone_books");
    xmlDocSetRootElement(doc, root);

    //向结点写入数据
    xmlNodePtr phone_node = xmlNewNode(NULL, BAD_CAST"phone");
    xmlNewProp(phone_node, BAD_CAST"id", BAD_CAST"1219");
    xmlNewProp(phone_node, BAD_CAST"nick", BAD_CAST"Two second");
    xmlNewChild(phone_node, NULL, BAD_CAST"name", BAD_CAST"aut");
    xmlNewChild(phone_node, NULL, BAD_CAST"mail", BAD_CAST"autyinjing@126.com");
    xmlAddChild(root, phone_node);
    /*xmlNewChild(root, NULL, BAD_CAST"mail", BAD_CAST"autyinjing@126.com");*/

    xmlNodePtr sub_node = xmlNewNode(NULL, BAD_CAST"phone_list");
    xmlAddChild(phone_node, sub_node);

    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST"phone_node");
    xmlNewChild(node, NULL, BAD_CAST"tel", BAD_CAST"138-8888-8888");
    xmlNewChild(node, NULL, BAD_CAST"address", BAD_CAST"ShangHai");
    xmlAddChild(sub_node, node);

    node = xmlNewNode(NULL, BAD_CAST"phone_node");
    xmlNewChild(node, NULL, BAD_CAST"tel", BAD_CAST"136-6666-6666");
    xmlNewChild(node, NULL, BAD_CAST"address", BAD_CAST"ShenZhen");
    xmlAddChild(sub_node, node);

    xmlSaveFormatFileEnc("new_phone_test.xml", doc, "UTF-8", 1);
    /*xmlSaveFile("phone_test_not_enc.xml", doc);*/
    xmlFreeDoc(doc);

    return 0;
}
```
执行程序后生成的new_phone_test.xml内容如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<phone_books>
  <phone id="1219" nick="Two second">
    <name>aut</name>
    <mail>autyinjing@126.com</mail>
    <phone_list>
      <phone_node>
        <tel>138-8888-8888</tel>
        <address>ShangHai</address>
      </phone_node>
      <phone_node>
        <tel>136-6666-6666</tel>
        <address>ShenZhen</address>
      </phone_node>
    </phone_list>
  </phone>
</phone_books>
```

##### 从xml文档解析
```
#include <stdio.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

int main(int argc, const char* argv[])
{
    xmlDocPtr   doc = NULL;     //文档指针
    xmlNodePtr root = NULL;     //根节点指针
    xmlNodePtr  cur = NULL;     //结点指针
    xmlChar     *id = NULL;

    /*doc = xmlParseFile("phone_test.xml");*/
    doc = xmlReadFile("new_phone_test.xml", NULL, XML_PARSE_NOBLANKS);  //支持更多选项，filename还可以是URL链接

    root = xmlDocGetRootElement(doc);
    cur = root->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"phone")))
        {
            id = xmlGetProp(cur, BAD_CAST"id");
            printf("id:%s\n", id);

            //解析子节点
            xmlNodePtr sub_ptr = NULL;
            xmlChar *key = NULL;
            sub_ptr = cur->xmlChildrenNode;
            while (sub_ptr != NULL)
            {
                if ((!xmlStrcmp(sub_ptr->name, (const xmlChar *)"name"))) {
                    key = xmlNodeListGetString(doc, sub_ptr->xmlChildrenNode, 1);
                    printf("name: %s\n", key);
                }
                if ((!xmlStrcmp(sub_ptr->name, (const xmlChar *)"mail"))) {
                    key = xmlNodeListGetString(doc, sub_ptr->xmlChildrenNode, 1);
                    printf("mail: %s\n", key);
                }

                xmlNodePtr  ssub_ptr = NULL;
                if ((!xmlStrcmp(sub_ptr->name, (const xmlChar *)"phone_list"))) {
                    printf("phone_list: \n");
                    ssub_ptr = sub_ptr->xmlChildrenNode;
                    while (ssub_ptr != NULL)
                    {
                        if ((!xmlStrcmp(ssub_ptr->name, (const xmlChar *)"phone_node"))) {
                            printf("  phone_node: \n");
                            xmlNodePtr sssub_ptr = ssub_ptr->xmlChildrenNode;
                            while (sssub_ptr != NULL)
                            {
                                if ((!xmlStrcmp(sssub_ptr->name, (const xmlChar *)"tel"))) {
                                    key = xmlNodeListGetString(doc, sssub_ptr->xmlChildrenNode, 1);
                                    printf("    tel: %s\n", key);
                                }
                                if ((!xmlStrcmp(sssub_ptr->name, (const xmlChar *)"address"))) {
                                    key = xmlNodeListGetString(doc, sssub_ptr->xmlChildrenNode, 1);
                                    printf("    address: %s\n", key);
                                }
                                sssub_ptr = sssub_ptr->next;
                            }
                        }
                        ssub_ptr = ssub_ptr->next;
                    }
                }
                sub_ptr = sub_ptr->next;
            }
        }
        cur = cur->next;
    }

    xmlFreeDoc(doc);
    return 0;
}
```
执行程序后输出结果为：
```
id:1219
name: aut
mail: autyinjing@126.com
phone_list: 
  phone_node: 
    tel: 138-8888-8888
    address: ShangHai
  phone_node: 
    tel: 136-6666-6666
    address: ShenZhen
```

### 注意点

    - 无论是新建文档，还是从文件解析，doc结构都需要申请内存空间，记得使用xmlFreeDoc()释放空间。