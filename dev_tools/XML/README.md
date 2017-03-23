
### XML实例

```
<?xml version="1.0"?>

<addressBook>
    <name>aut</name>
    <mail>autyinjing@126.com</mail>
    <phone_list>
        <phone format="type/number">
            <type>home</type>
            <number>02188888888</number>
        </phone>
        <phone format="type/number">
            <type>mobile</type>
            <number>13888888888</number>
        </phone>
    </phone_list>
</addressBook>
```

#### 解释
例子 | 说明
--- | ---
<?xml version="1.0"?> | 版本声明
addressBook | 根节点
name | 一级子节点
phone | 二级子节点
format | 结点属性
type | 三级子节点
home | 节点值
<xxx> | 打开标签
</xxx> | 关闭标签