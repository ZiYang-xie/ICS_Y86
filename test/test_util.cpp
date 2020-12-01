//
// Created by 谢子飏 on 2020/12/01.
//

#include "test_util.h"

// 翻转字符，输入偶长度字符串，每两位相对位置保持不变，翻转字符串
// 用以翻转小端法
std::string reverse_str(std::string s)
{
    if(s.length() & 1)
        throw std::domain_error("s must have even length");
    if(s == "")
        return "";
    else {
        std::string tmp;
        tmp.assign(s, 0, 2);
        s.erase(0, 2);
        return reverse_str(s) + tmp;
    }
}

// 将字符串翻转并转化为uint64_t型输出
// 注意: 改函数并不会处理非法输入，当字符串中包含非数字字符时会自动忽略
// 若不存在数字字符，则输出为0
uint8_t str2int(const std::string& s)
{
    uint64_t res;
    std::stringstream str2digit; // 定义字符流
    str2digit << reverse_str(s);
    str2digit >> res;
    return res;
}

// 将int转为string，补齐16位并翻转
std::string int2str(const uint8_t& n)
{
    std::string s = std::to_string(n);
    while(s.length() < 16)
        s = "0" + s;
    return reverse_str(s);
}

// 往内存写8bytes
void write_8_bytes(Device::Device& d, const uint8_t& start_pos, const uint8_t& val)
{
    while(val > 0)
    {
        d.Mem[start_pos] = val & 0xff;
        val >>= 8;
        start_pos += 1;
    }
}

// 从内存读8bytes
uint64_t read_8_bytes(Device::Device& d, const uint8_t& start_pos)
{
    uint64_t res = 0;
    // 这就是程序优化吗
    int tmp = 0;
    for(int i = 0; i < 8; ++i)
    {
        res += d.Mem[start_pos + i] << tmp;
        tmp += 8;
    }
    return res;
}
