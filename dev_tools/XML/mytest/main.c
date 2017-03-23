/* ***********************************************************************

  > File Name: main.c
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年03月23日 星期四 11时17分47秒

 ********************************************************************** */

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
