# 代码规范约定

vscode中, 我的format设定为`{ BasedOnStyle: Google, IndentWidth: 4 }`

## 语言

为了使表意清晰且便于助教阅读, 本次项目中包括但不限于注释、文档、Git提交说明请使用中文

## C++标准

本次项目将使用C++17作为语言标准, 但是不会过度使用模板黑魔法和新特性

## 命名规范

- 文件: c++文件以.cpp结尾, 头文件以.h结尾, 全小写形式加下划线, 如: my_useful_class.cpp

- 类: 每个单词开头大写, 如: MyClass

- 普通变量: 全小写形式加下划线, 如: my_variable_name

- 类的变量: 普通变量后加下划线, 如: my_class_variable_name_(Struct仅允许使用Plain Data, 因此变量命名和普通变量一样)

- 常量: 用const和constexpr声明的, 使用第一个字母为k加上每个单词开头大写, 如kMyConst

- 函数: 每个单词开头大写, 如: MyFunction(int my_data)

## 注释

- 使用`//`进行注释(vscode中快捷键cmd+/)

- 函数和类的**声明**前应当写清函数和类的作用(并且默认值也需要写在声明处), 这是为了后期调库时足够方便, 例如:

```c++
using Type = enum{kHex, kDec};
// 返回表示该MyVector的大小的字符串
// type为kHex时, 返回16进制的形式(含前缀0x)；type为kDec时, 返回10进行的形式(无前缀)
// 注意：该大小不是实际占用内存空间的大小, 而是申请内存空间中已用的大小
std::string MyVector::LengthInString(Type type);
```

- 其余地方可以适当的增加注释

## 其他

本代码规范约定参考了谷歌的C++代码规范[Google Style Guide](https://google.github.io/styleguide/cppguide.html), 如果有任何补充或者建议可以直接在上面修改, 谢谢子飏！
