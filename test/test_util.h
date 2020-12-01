//
// Created by 谢子飏 on 2020/12/01.
//

#ifndef ICS_Y86_TEST_UTIL_H
#define ICS_Y86_TEST_UTIL_H
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <string>
#include "../src/Device.h"

std::string reverse_str(std::string);
uint8_t str2int(const std::string&);
std::string int2str(const uint8_t&);
//void write_8_bytes(Device::Device&, const uint8_t&, const uint8_t&);
//uint64_t read_8_bytes(Device::Device&, const uint8_t&);

#endif