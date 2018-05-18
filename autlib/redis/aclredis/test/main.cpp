/* ***********************************************************************

  > File Name: main.cpp
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: Thu 17 May 2018 11:54:24 PM CST

 ********************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "acl_cpp/lib_acl.hpp"

static void test_redis_string(acl::redis& cmd, const char* key)
{
	acl::string val("test_value");

	// call redis-server: SET key value
	if (cmd.set(key, val.c_str()) == false)
	{
		printf("redis set error\r\n");
		return;
	}

	// clear the string buf space
	val.clear();

	// reset the redis command object for reusing it
	cmd.clear();

	// call redis-server: GET key
	acl::string val1;
	if (cmd.get(key, val1) == false)
		printf("get key error\r\n");
	printf("val: %s\r\n", val1.c_str());
}

static void test_redis_key(acl::redis& cmd, const char* key)
{
	if (cmd.exists(key) == false)
		printf("key not exists\r\n");
	else
		printf("key exists\r\n");
}

int main(void)
{
	// init socket module for windows
	//acl::acl_cpp_init();

	const char* redis_addr = "127.0.0.1:7000";
	int conn_timeout = 10, rw_timeout = 10, max_conns = 100;

	// declare redis cluster ojbect
	acl::redis_client_cluster cluster;
	cluster.set(redis_addr, max_conns, conn_timeout, rw_timeout);

	// redis operation command
	acl::redis cmd;

	// bind redis command with redis cluster
	cmd.set_cluster(&cluster, max_conns);

	const char* key = "test_key";

	// call redis server
	test_redis_string(cmd, key);
	test_redis_key(cmd, key);

	return 0;
}
