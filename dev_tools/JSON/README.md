
### 是什么

    - JSON 指的是 JavaScript 对象表示法（JavaScript Object Notation）
    - JSON 是轻量级的文本数据交换格式
    - JSON 独立于语言
    - JSON 具有自我描述性，更易理解
    
官方地址：http://www.json.org/

### 语法格式

```
JSON_obj = {
    "employees": 
    [
        { "firstName":"Bill" , "lastName":"Gates" },
        { "firstName":"George" , "lastName":"Bush" },
        { "firstName":"Thomas" , "lastName":"Carter" }
    ]
}
```

##### 说明

    - JSON建构于两种结构："key/value"集合和数组；
    - 一个JSON对象可以是一个"key/value"集合，或是一个数组；
    - key必须是一个字符串；
    - value可以是两种结构之一，或者是基础类型；
    - 数组元素也可以是两种类型之一，或者是基础元素。

##### 解释
    
    - JSON_obj是一个JSON对象，其中只包含一个子节点；
    - 该子节点是一个"key/value"集合，集合内只有一个元素；
    - 元素的key为employees，value为一个数组；
    - 数组包含三个元素，元素类型为"key/value"集合；
    - 每个集合中包含两个"key/value"对；
    - 两种结构之间用','分隔；
    - key和value都要用双引号括起来；
    - key和value之间用':'分隔。
