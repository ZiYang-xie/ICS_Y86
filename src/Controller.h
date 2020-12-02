//
// Created by 王少文 on 2020/12/1.
//

#ifndef ICS_Y86_CONTROLLER_H
#define ICS_Y86_CONTROLLER_H
#include "Device.h"
#include <string>
#include <iostream>
class Controller {
private:
  std::string scopy;
public:
  //TODO:后期再放进Private
  Device d;
  //将s写入Mem，同时还原所有的寄存器
  //若写入成功，则返回True，否则返回False
 bool FlashCode(std::string s);
 //还原所有的寄存器
 void Reset();
 //运行内部的代码，直到异常情况停止
 void Run(std::ostream &os = std::cout);
};

#endif // ICS_Y86_CONTROLLER_H
