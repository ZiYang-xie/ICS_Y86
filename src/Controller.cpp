//
// Created by 王少文 on 2020/12/1.
//

#include "Controller.h"

#include <utility>

#include "Device.h"
#include "Output.h"
#include "cstring"

bool Controller::FlashCode(std::string s) {
    if (s.length() < MEM_SIZE * 2) {
        scopy = s;
        d = Device(std::move(s));
        return true;
    } else {
        return false;
    }
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
std::string Controller::GetConsoleOutput() {
    char dst[0x100] = {0};
    memcpy(dst, d.Mem + CONSOLE_MEM_START, CONSOLE_MEM_SIZE);
    return std::string(dst);
}
