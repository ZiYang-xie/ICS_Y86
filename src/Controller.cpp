//
// Created by 王少文 on 2020/12/1.
//

#include "Controller.h"

#include <utility>

#include "Output.h"

bool Controller::FlashCode(std::string s) {
    scopy = s;
    d = Device(std::move(s));
    return true;  //来不及写异常处理了就先这样吧
}
void Controller::Reset() { d = Device(scopy); }
void Controller::Run(int max_cycle, bool if_output, std::ostream& os) {
    int idx = 0;
    while (d.Stat == SAOK) {
        d.Fetch();
        d.Decode();
        d.Execute();
        d.Memory();
        d.Writeback();
        d.SetCC();
        d.SetDSrcA();
        d.SetDSrcB();
        d.SetFControl();
        d.SetDControl();
        d.SetEControl();
        d.F2D();
        d.D2E();
        d.E2M();
        d.M2W();
        if (if_output) {
            OutputProcedure(os, idx, this->d);
        }
        if (idx++ > max_cycle) {
            std::cerr << "Exceed max cycle: " << max_cycle;
            break;
        }
    }
}
