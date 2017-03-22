

### 项目构建流程

1. 创建工程目录project_dir
2. 在project_dir中创建以下(常用)目录：
    - src：存放源码
    - conf：存放配置文件
    - bin：存放目标文件
    - doc：存放文档
    - build：作为cmake的构建目录
3. 编写代码及配置文件
4. 在project_dir及各源码目录编写CMakeLists.txt
5. 在build中cmake、make生成目标文件


### CMake基础知识

官方地址：++https://cmake.org/++

1. 使用方法：
    1. 编写CMakeLists.txt文件；
    2. cmake SRC_DIR 生成Makefile；
    3. make 编译。

2. 核心文件：CMakeLists.txt

3. CMakeLists.txt语法  
    - CMakeLists.txt由命令、注释和空格组成；
    - 命令不区分大小写；
    - 符号"#"后面的内容是注释；
    - 命令由命令名称、小括号和参数组成,参数之间使用空格进行间隔。

4. 内部构建与外部构建
    - 内部构建：在源码目录进行编译；
    - 外部构建：在源码目录之外的目录进行编译；

5. CMake主要功能
    - 添加子目录；
    - 添加模块；
    - 生成库文件；
    - 安装；
    - 测试；
    - 生成目标文件。

6. CMakeLists.txt文件框架
```
# 文件说明
# ...

# 项目名称和CMake版本要求 - 必选
PROJECT()
CMAKE_MINIMUM_REQUIRED()

# 设置相关变量和选项 - 比如编译器、模块路径、编译选项等
SET()
OPTION()
ADD_DEFINITIONS()
# ...

# 添加目录和代码文件
ADD_SUBDIRECTORY()
AUX_SOURCE_DIRECTORY()
...

# 添加外部模块
FIND_PACKAGE()
# ...

# 生成目标文件
INCLUDE_DIRECTORIES()
TARGET_LINK_LIBRARIES()
ADD_LIBRARY() / ADD_EXECUTABLE()
# ...

# 安装到指定目录 - 可选
INSTALL()
# ...

# 添加测试文件 - 可选
ADD_TEST()
ENABLE_TESTING()
# ...

```

7. Findxxx.cmake文件框架
```

# 查找头文件
FIND_PATH()

# 查找库文件
FIND_LIBRARY()

# 根据查找结果设置相关变量
SET()

# 根据查找结果输出提示信息或者终止编译
IF ()
    MESSAGE()
ELSE ()
    MESSAGE()
ENDIF ()

```

### 其他

shell命令 | 功能
--- | ---
make clean | 清理构建结果
make install | 安装
make test | 执行测试

### 常用命令列表

#### 基础命令
    PROJECT - 指定项目名称
    CMAKE_MINIMUM_REQUIRED - 指定CMake版本
    SET - 显式定义变量
    MESSAGE - 向终端输出消息

#### 编译相关
    ADD_EXECUTABLE - 指定生成可执行文件
    ADD_SUBDIRECTORY - 添加子目录
    AUX_SOURCE_DIRECTORY - 追加源文件到变量中
    ADD_LIBRARY - 生成库文件
    TARGET_LINK_LIBRARIES - 链接共享库
    ADD_DEFINITIONS - 添加编译选项
    INCLUDE_DIRECTORIES - 添加头文件搜索路径

#### 其他
    INSTALL - 安装
    SET_TARGET_PROPERTIES - 设置目标文件的属性
    GET_TARGET_PROPERTY - 获取目标文件的属性
    ADD_TEST - 添加测试
    ENABLE_TESTING - 使所有TEST生效
    OPTION - 为选项设置开关


### 常用变量
变量名 | 意义
--- | ---
PROJECT_BINARY_DIR | 编译路径
PROJECT_SOURCE_DIR | 工程路径
EXECUTABLE_OUTPUT_PATH | 可执行文件的输出路径
LIBRARY_OUTPUT_PATH | 库文件的输出路径
CMAKE_INSTALL_PREFIX | 安装文件的目标路径前缀
CMAKE_MODULE_PATH | cmake模块所在的路径
DESTINATION | 安装路径