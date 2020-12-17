//
// Created by 王少文 on 2020/11/28.
//
// 本文件没有取名为utility.h是为了避免和标准库命名冲突

#ifndef ICS_Y86_UTIL_H
#define ICS_Y86_UTIL_H
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

// In函数的递归终止函数，请不要直接调用
template <typename T>
bool In(T lhs, T first) {
    return lhs == first;
}
// In函数模拟了Python中in的使用方法，第一个参数为需要被比较的值，后面的参数为列表，当值在列表中时，返回True，否则返回False
// 例如: In(1,2,3,4,5) = False, In(1,2,3,1,5) = True
// 注意: 所有的参数需为同一类型
template <typename T, typename... List>
bool In(T lhs, T first, List... list) {
    return lhs == first || In(lhs, list...);
}

// CharToUint8，将Char型数据转化为8位的无符号整数。
// 合理的输入取值范围为 0-9 a-f A-F，建议使用小写字母以保持统一
// 若不在该范围内，会抛出异常
uint8_t CharToUint8(char c);

// 提前实现C++20中的std::format功能
// 用法和Python的str%format很像
// Ref:
// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template <typename... Args>
std::string Format(const std::string& format_string, Args... args) {
    int size = snprintf(nullptr, 0, format_string.c_str(), args...) +
               1;  // Extra space for '\0'
    if (size <= 0) {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format_string.c_str(), args...);
    return std::string(buf.get(),
                       buf.get() + size - 1);  // We don't want the '\0' inside
}

#endif  // ICS_Y86_UTIL_H
