
#### 博客框架
是什么？  
为什么要用？（优点及适用场景）  
下载和安装  
文件结构（.proto 和 生成的文件）  
例子开始（import、package、message、enum、field type、嵌套）  
生成 xxxx.pb.h 和 xxxx.pb.cc 文件  
使用生成的接口  
编译和链接  
常用API及生成的接口  

protobuf编码（可开新文章）  
.proto文件编码风格  
其他细节

#### 博客具体内容

注：下面的操作以C++为编程语言，protoc的版本为libprotoc 3.2.0。

一、Protobuf是什么东西？  
1. 是什么？  
Google Protocol Buffer(简称 Protobuf)是一种轻便高效的结构化数据存储格式，平台无关、语言无关、可扩展，可用于通讯协议和数据存储等领域。

2. 为什么要用？
    1. 平台无关，语言无关，可扩展；
    2. 提供了友好的动态库，使用简单；
    3. 解析速度快，比对应的XML快约20-100倍；
    4. 序列化数据非常简洁、紧凑，与XML相比，其序列化之后的数据量约为1/3到1/10；

3. 怎么安装？  
源码下载地址：++https://github.com/google/protobuf++  
安装依赖的库：autoconf automake libtool curl make g++ unzip  
安装：
    - $ ./autogen.sh
    - $ ./configure
    - $ make
    - $ make check
    - $ sudo make install

二、怎么用？  
1. 编写proto文件  
首先需要一个proto文件，其中定义了我们程序中需要处理的结构化数据：  
```c
// Filename: addressbook.proto

syntax="proto2";
package addressbook;

import "src/help.proto";    

message Person {
    required string name  = 1;
    required int32 id     = 2;
    optional string email = 3;
    
    enum PhoneType {
        MOBILE = 0;
        HOME   = 1;
        WORK   = 2;
    }
    
    message PhoneNumber {
        required string number  = 1;
        optional PhoneType type = 2 [default = HOME];
    }
    
    repeated PhoneNumber phone  = 4;
}

message AddressBook {
    repeated Person person_info = 1;
}
```
2. 代码解释  

``` // Filename: addressbook.proto ```这一行是注释，语法类似于C++  
``` syntax="proto2"; ```表明使用protobuf的编译器版本为v2，目前最新的版本为v3  
``` package addressbook; ```声明了一个包名，用来防止不同的消息类型有命名冲突，类似于Cpp中的``` namespace ```  
``` import "src/help.proto"; ```导入了一个外部proto文件中的定义，类似于Cpp中的``` include ```。不过好像只能import当前目录及当前目录的子目录中的proto文件，比如import父目录中的文件时编译会报错（Import "../xxxx.proto" was not found or had errors.），使用绝对路径也不行，尚不清楚原因，官方文档说使用"-I=PATH"或者"--proto_path=PATH"来指定import目录，但实际实验结果表明这两种方式指定的是将要编译的proto文件所在的目录，而不是import的文件所在的目录。（哪位大神若清楚还请不吝赐教！）  
``` message ```是Protobuf中的结构化数据，类似于Cpp中的类，可以在其中定义需要处理的数据  
``` required string name  = 1; ```声明了一个名为name，数据类型为string的required型字段，该字段的标识号为1  
protobuf一共有三个字段修饰符：  
    - required：该值是必须要设置的；  
    - optional：该字段可以有0个或1个值（不超过1个）；  
    - repeated：该字段可以重复任意多次（包括0次），类似于Cpp中的list；  

使用建议：除非确定某个字段一定会被设值，否则使用optional代替required。  
``` string ```是一种标量类型，protobuf的所有标量类型请参考文末的标量类型列表。  
``` name ```是字段名，1 是字段的标识号，在消息定义中，每个字段都有唯一的一个数字标识号，这些标识号是用来在消息的二进制格式中识别各个字段的，一旦开始使用就不能够再改变。  
标识号的范围在：1 ~ 2^29 - 1，其中[19000－19999]为Protobuf预留，不能使用。
``` Person ```内部声明了一个enum和一个message，这类似于Cpp中的类内声明，Person外部的结构可以用``` Person.PhoneType ```的方式来使用PhoneType。当使用外部package中的结构时，要使用``` pkgName.msgName.typeName ```的格式，每两层之间使用'.'来连接，类似Cpp中的"::"。  
``` optional PhoneType type = 2 [default = HOME]; ```为type字段指定了一个默认值，当没有为type设值时，其值为HOME，记住：只能为optional字段设置默认值。  
另外，一个proto文件中可以声明多个message，在编译的时候他们会被编译成为不同的类。

3. 生成Cpp文件  
protoc是proto文件的编译器，目前可以将proto文件编译成Cpp、Java、Python三种代码文件，编译格式为：  
``` 
protoc -I=$SRC_DIR --cpp_out=$DST_DIR /path/to/file.proto
```
上面的命令会生成xxx.pb.h 和 xxx.pb.cc两个Cpp文件（注意：如果没有src/help.proto文件，去掉import那一行）。  

4. 使用Cpp文件  

现在编写一个main.cc文件：
```c
#include <iostream>
#include "addressbook.pb.h"

int main(int argc, const char* argv[])
{
    addressbook::AddressBook person;
    addressbook::Person* pi = person.add_person_info();

    pi->set_name("aut");
    pi->set_id(1219);
    std::cout << "before clear(), id = " << pi->id() << std::endl;
    pi->clear_id();
    std::cout << "after  clear(), id = " << pi->id() << std::endl;
    pi->set_id(1087);
    if (!pi->has_email())
        pi->set_email("autyinjing@126.com");

    addressbook::Person::PhoneNumber* pn = pi->add_phone();
    pn->set_number("021-8888-8888");
    pn = pi->add_phone();
    pn->set_number("138-8888-8888");
    pn->set_type(addressbook::Person::MOBILE);

    uint32_t size = person.ByteSize();
    unsigned char byteArray[size];
    person.SerializeToArray(byteArray, size);

    addressbook::AddressBook help_person;
    help_person.ParseFromArray(byteArray, size);
    addressbook::Person help_pi = help_person.person_info(0);

    std::cout << "*****************************" << std::endl;
    std::cout << "id:    " << help_pi.id() << std::endl;
    std::cout << "name:  " << help_pi.name() << std::endl;
    std::cout << "email: " << help_pi.email() << std::endl;

    for (int i = 0; i < help_pi.phone_size(); ++i)
    {
        auto help_pn = help_pi.mutable_phone(i);
        std::cout << "phone_type: " << help_pn->type() << std::endl;
        std::cout << "phone_number: " << help_pn->number() << std::endl;
    }
    std::cout << "*****************************" << std::endl;

    return 0;
}  
```
5. 常用API  

首先，需要包含生成的xxx.pb.h文件。  
protoc为message的每个required字段和optional字段都定义了以几个函数（不限于这几个）：
```c
TypeName xxx() const;           //获取字段的值
bool has_xxx();                 //判断是否设值
void set_xxx(const TypeName&);  //设值
void clear_xxx();               //使其变为默认值
```
为每个repeated字段定义了以下几个：
```c
TypeName* add_xxx();            //增加结点
TypeName xxx(int) const;        //获取指定序号的结点，类似于Cpp的"[]"运算符
TypeName* mutable_xxx(int);     //类似于上一个，但是获取的是指针
int xxx_size();                 //获取结点的数量
```
另外，下面几个是常用的序列化函数：
```c
bool SerializeToOstream(std::ostream * output) const;   //输出到输出流中
bool SerializeToString(string * output) const;          //输出到string
bool SerializeToArray(void * data, int size) const;     //输出到字节流
```
与之对应的反序列化函数：
```c
bool ParseFromIstream(std::istream * input);            //从输入流解析
bool ParseFromString(const string & data);              //从string解析
bool ParseFromArray(const void * data, int size);       //从字节流解析
```
其他常用的函数：
```c
bool IsInitialized();       //检查是否所有required字段都被设值
size_t ByteSize() const;    //获取二进制字节序的大小
```
官方API文档地址：++https://developers.google.com/protocol-buffers/docs/reference/overview++

6. 编译生成可执行代码  

编译格式和普通的Cpp代码一样，但是要加上"-lprotobuf -pthread"：
```c
g++ main.cc xxx.pb.cc -I $INCLUDE_PATH -L $LIB_PATH -lprotobuf -pthread
```

7. 输出结果  
```c
before clear(), id = 1219
after  clear(), id = 0
*****************************
id:    1087
name:  aut
email: autyinjing@126.com
phone_type: 1
phone_number: 021-8888-8888
phone_type: 0
phone_number: 138-8888-8888
*****************************
```

三、怎么编码的？  

protobuf之所以小且快，就是因为使用变长的编码规则，只保存有用的信息，节省了大量空间。
1. Base-128变长编码
    - 每个字节使用低7位表示数字，除了最后一个字节，其他字节的最高位都设置为1；
    - 采用Little-Endian字节序。

示例：
```c
-数字1：
0000 0001

-数字300：
1010 1100   0000 0010
    000 0010    010 1100
-> 000 0010  010 1100
-> 100101100
-> 256 + 32 + 8 + 4 = 300
```

2. ZigZag编码

Base-128变长编码会去掉整数前面那些没用的0，只保留低位的有效位，然而负数的补码表示有很多的1，所以protobuf先用ZigZag编码将所有的数值映射为无符号数，然后使用Base-128编码，ZigZag的编码规则如下：
```
(n << 1) ^ (n >> 31) or (n << 1) ^ (n >> 63)
```
负数右移后高位全部变成1，再与左移一位后的值进行异或，就把高位那些无用的1全部变成0了，真的够巧妙！

3. 消息格式

每一个Protocol Buffers的Message包含一系列的字段（key/value），每个字段由字段头（key）和字段体（value）组成，字段头由一个变长32位整数表示，字段体由具体的数据结构和数据类型决定。  
    
字段头格式：
```c
(field_number << 3) | wire_type
-field_number：字段序号
-wire_type：字段编码类型
```

4. 字段编码类型  
Type	Meaning	Used For  
0	Varint	int32, int64, uint32, uint64, sint32, sint64, bool, enum  
1	64-bit	fixed64, sfixed64, double  
2	Length-delimited	string, bytes, embedded messages（嵌套message）, packed repeated fields  
3	Start group	groups (废弃)  
4	End group	groups (废弃)  
5	32-bit	fixed32, sfixed32, float  

5. 编码示例（下面的编码以16进制表示）
```c
示例1（整数）
message Test1 {
    required int32 a = 1;
}
a = 150 时编码如下
08 96 01
08:     1 << 3 | 0
96 01:
1001 0110   0000 0001
-> 001 0110     000 0001
-> 1001 0110
-> 150

示例2（字符串）
message Test2 {
    required string b = 2;
}
b = "testing" 时编码如下
12 07 74 65 73 74 69 6e 67
12:     2 << 3 | 2
07:     字符串长度
74 65 73 74 69 6e 67
-> t e s t i n g

示例3（嵌套）
message Test3 {
    required Test1 c = 3;
}
c.a = 150 时编码如下
1a 03 08 96 01
1a：    3 << 3 | 2
03：    嵌套结构长度
08 96 01
->Test1 { a = 150 }

示例4（可选字段）
message Test4 {
    required int32 a  = 1;
    optional string b = 2;
}
a = 150, b不设值时编码如下
08 96 01
-> { a = 150 }

a = 150, b = "aut" 时编码如下
08 96 01 12 03 61 75 74
08 96 01 -> { a = 150 }
12：    2 << 3 | 2
03：    字符串长度
61 75 74
-> a u t

示例5（重复字段）
message Test5 {
    required int32 a  = 1;
    repeated string b = 2;
}
a = 150, b = {"aut", "honey"} 时编码如下
08 96 01 12 03 61 75 74 12 05 68 6f 6e 65 79
08 96 01 -> { a = 150 }
12：    2 << 3 | 2
03：    strlen("aut") 
61 75 74 -> a u t
12：    2 << 3 | 2
05：    strlen("honey")
68 6f 6e 65 79 -> h o n e y

a = 150, b = "aut" 时编码如下
08 96 01 12 03 61 75 74
08 96 01 -> { a = 150 }
12：    2 << 3 | 2
03：    strlen("aut") 
61 75 74 -> a u t

示例6（字段顺序）
message Test6 {
    required int32 a  = 1;
    required string b = 2;
}
a = 150, b = "aut" 时，无论a和b谁的声明在前面，编码都如下
08 96 01 12 03 61 75 74
08 96 01 -> { a = 150 }
12 03 61 75 74 -> { b = "aut" }
```

四、还有什么？  

1. 编码风格  
    - 花括号的使用(参考上面的proto文件)
    - 数据类型使用驼峰命名法：AddressBook, PhoneType
    - 字段名小写并使用下划线连接：person_info, email_addr
    - 枚举量使用大写并用下划线连接：FIRST_VALUE, SECOND_VALUE

2. 适用场景  

"Protocol Buffers are not designed to handle large messages."。protobuf对于1M以下的message有很高的效率，但是当message是大于1M的大块数据时，protobuf的表现不是很好，请合理使用。

总结：本文介绍了protobuf的基本使用方法和编码规则，还有很多内容尚未涉及，比如：反射机制、扩展、Oneof、RPC等等，更多内容请参考官方文档。

#### 标量类型列表  
proto类型   C++类型     备注  
double       double  
float        float  
int32        int32      使用可变长编码方式。编码负数时不够高效——如果你的字段可能含有负数，那么请使用sint32。  
int64        int64      使用可变长编码方式。编码负数时不够高效——如果你的字段可能含有负数，那么请使用sint64。  
uint32       uint32     使用可变长编码方式  
uint64       uint64	    使用可变长编码方式  
sint32       int32      使用可变长编码方式。有符号的整型值。编码时比通常的int32高效。  
sint64       int64      使用可变长编码方式。有符号的整型值。编码时比通常的int64高效。  
fixed32      uint32     总是4个字节。如果数值总是比总是比228大的话，这个类型会比uint32高效。  
fixed64      uint64     总是8个字节。如果数值总是比总是比256大的话，这个类型会比uint64高效。  
sfixed32     int32      总是4个字节。  
sfixed64     int64      总是8个字节。  
bool         bool  
string       string     一个字符串必须是UTF-8编码或者7-bit ASCII编码的文本。  
bytes        string     可能包含任意顺序的字节数据。  
官方最新参考：++https://developers.google.com/protocol-buffers/docs/encoding++

#### 参考资料  
Protocol Buffers Developer Guide  
https://developers.google.com/protocol-buffers  

Google Protocol Buffer 的使用和原理
https://www.ibm.com/developerworks/cn/linux/l-cn-gpb/

浅谈几种序列化协议  
http://yaocoder.blog.51cto.com/2668309/795807

序列化和反序列化  
http://tech.meituan.com/serialization_vs_deserialization.html  

Protobuf使用手册  
http://linghutf.github.io/2016/06/08/protobuf/ 

（本文完）