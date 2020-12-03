//
// Created by 王少文 on 2020/11/28.
//
#include "util.h"
uint8_t CharToUint8(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  } else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  } else {
    throw std::domain_error("c must be 0-9 or a-f or A-F");
    exit(1);
  }
}




