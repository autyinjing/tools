/* ***********************************************************************

  > File Name: parse_main.c
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年03月23日 星期四 16时12分15秒

 ********************************************************************** */

#include <stdio.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifndef DO_FAILED
#define DO_FAILED() \
    if (doc) \
        xmlFreeDoc(doc); \
    return -1;
#endif

/*XML_PARSE_NOBLANKS*/
/*xmlReadFile*/

int main(int argc, const char* argv[])
{
    xmlDocPtr   doc = NULL;     //文档指针
    xmlNodePtr root = NULL;     //根节点指针
    xmlNodePtr  cur = NULL;     //结点指针
    xmlChar     *id = NULL;

    /*doc = xmlParseFile("phone_test.xml");*/
    doc = xmlReadFile("new_phone_test.xml", NULL, XML_PARSE_NOBLANKS);  //支持更多选项，filename还可以是URL链接

    root = xmlDocGetRootElement(doc);
    if ((xmlStrcmp(root->name, (const xmlChar *)"phone_books"))) {
        fprintf(stderr, "Root node error.\n");
        DO_FAILED();
    }

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

