//
// Created by 王少文 on 2020/12/1.
//

#include "Controller.h"

#include <utility>
bool Controller::FlashCode(std::string s) {
  scopy = s;
  d = Device(std::move(s));
  return true; //来不及写异常处理了就先这样吧
}
void Controller::Reset() { d = Device(scopy); }
void Controller::Run(std::ostream &os) {
  while (d.Stat == Device::SAOK) {
    d.Fetch();
    d.Decode();
    d.Execute();
    d.Memory();
    d.Writeback();
    for(unsigned long long i : d.Reg){
      os<<i<<' ';
    }
  }
}
