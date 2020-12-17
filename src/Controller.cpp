//
// Created by 王少文 on 2020/12/1.
//

#include "Controller.h"

#include <utility>
#include <vector>
#ifdef OUTPUT_JSON
#include "../library/json.hpp"
#endif
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
void Controller::Run(bool if_output, int max_cycle, std::ostream& os) {
    int idx = 1;
#ifdef OUTPUT_JSON
    std::vector<json> cycle_vec;
#endif
    while (d.Stat == SAOK) {
        d.Fetch();
        d.Decode();
        d.Execute();
        d.Memory();
        d.Writeback();
        d.UpdateIfJumpState();
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
#ifdef OUTPUT_JSON
            // OutputToJsonCycle(this->d);
            cycle_vec.push_back(OutputToJsonCycle(this->d));
#else
            OutputProcedure(os, idx, this->d);
#endif
        }

        if (idx++ > max_cycle) {
            std::cerr << "Exceed max cycle: " << max_cycle;
            break;
        }
    }
#ifdef OUTPUT_JSON
    OutputToJsonFinal(idx, idx * 0.8, cycle_vec, os);
#endif
}
std::string Controller::GetConsoleOutput() {
    char dst[0x100] = {0};
    memcpy(dst, d.Mem + CONSOLE_MEM_START, CONSOLE_MEM_SIZE);
    return std::string(dst);
}
