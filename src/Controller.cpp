//
// Created by 王少文 on 2020/12/1.
//

#include "Controller.h"
#include "infoList.h"

#include <utility>

bool Controller::FlashCode(std::string s) {
    scopy = s;
    d = Device(std::move(s));
    return true;  //来不及写异常处理了就先这样吧
}
void Controller::Reset() { d = Device(scopy); }
void Controller::Run(std::ostream &os) {
    int idx = 0;
    while (d.Stat == Device::SAOK) {
        d.Fetch();
        d.Decode();
        d.Execute();
        d.Memory();
        d.Writeback();
        d.SetFControl();
        d.SetDControl();
        d.SetEControl();
        d.F2D();
        d.D2E();
        d.E2M();
        d.M2W();
        Output(os, idx);
    }
}

void Controller::Output(std::ostream & os, int& idx) {
    os << "Cycle " << std::dec << idx++ << ": " << std::endl;
    for (unsigned long long i = 0; i < REG_SIZE; ++i) {
        os << RegList[i] << ":\t0x" << std::hex << i << "  ";
        if(!(i & 1))
            os << std::endl;
    }
    os << std::endl;
}
