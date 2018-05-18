
### aclredis使用笔记

- 注：本仓库已经将lib和include整理出来，使用时直接拷贝就可以

1. 下载和安装
```
# 下载源码
$ git clone https://github.com/zhengshuxin/acl.git

# 编译相关的库
# *** libacl.a ***
$ cd $ACL_PATH/lib_acl
$ make
$ cp lib/*.a $LIB_PATH

# *** libprotocol.a ***
$ cd $ACL_PATH/lib_protocol
$ make
$ cp lib/*.a $LIB_PATH

# *** libacl_cpp.a ***
$ cd $ACL_PATH/lib_acl_cpp
$ make
$ cp lib/*.a $LIB_PATH

# 拷贝相关头文件
$ cp -R $ACL_PATH/lib_acl_cpp/include/* $INCLUE_PATH
```

2. 编写测试用例
```
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
    acl::acl_cpp_init();
           
    const char* redis_addr = "127.0.0.1:6379";
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
```

3. 编译和测试
```
# Makefile
CFLAGS = -c -g -W -O3 -Wall -Werror -nostartfiles -Wshadow \
         -Wno-long-long -Wpointer-arith -D_REENTRANT \
         -D_POSIX_PTHREAD_SEMANTICS -DLINUX2 \
         -I ./include
BASE_PATH=./        
LDFLAGS = -L$(BASE_PATH)/lib -lacl_cpp \
          -L$(BASE_PATH)/lib -lprotocol \
          -L$(BASE_PATH)/lib -lacl \
          -lpthread 
test: main.o        
        g++ -o test main.o $(LDFLAGS)                       
main.o: main.cpp    
        g++ $(CFLAGS) main.cpp -o main.o
```
