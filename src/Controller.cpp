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
    os << std::endl;
    os << "Cycle " << std::dec << idx++ << ": " << "\n\n";
    for (unsigned long long i = 0; i < REG_SIZE; ++i) {
        os << RegList[i] << ":\t0x" << std::hex << d.Reg[i] << "  ";
        if(!(i & 1))
            os << std::endl;
    }
    os << "\n\n";

    os << "F_Reg:\nControl: " << ControlList[d.F.control] << "\t\t" << "predPC: 0x" << std::hex << d.F.predPC << "\n\n";
    
    os << "D_Reg:\nControl: " << ControlList[d.D.control] << "\t\t" << "Stat: " << StatList[d.D.stat] << "\t";
    if(d.D.icode | d.D.ifun)
        os << "icode: " << std::hex << d.D.icode << "\t" << "ifun: " << d.D.ifun << "\t";
    os << "rA: " << RegList[d.D.rA] << "\t\t" << "rB: " << RegList[d.D.rB] << "\t\t";
    os << "valC: 0x" << std::hex << d.D.valC << "\t\t" << "valP: 0x" <<  d.D.valP << "\n\n";

    os << "E_Reg:\nControl: " << ControlList[d.E.control] << "\t\t" << "Stat: " << StatList[d.E.stat] << "\t";
    if(d.E.icode | d.E.ifun)
        os << "icode: " << std::hex << d.E.icode << "\t" << "ifun: " << d.E.ifun << "\t";
    os << "valC: 0x" << std::hex << d.E.valC << "\t" << "valA: 0x" <<  d.E.valA << "\t" << "valB: 0x" <<  d.E.valB << "\t";
    os << "srcA: " << RegList[d.E.srcA] << "\t" << "srcB: " << RegList[d.E.srcB] << "\t" << "dstE: " << RegList[d.E.dstE] << "\t" << "detM: " << RegList[d.E.dstM] << "\n\n";

    os << "M_Reg:\n" << "Stat: " << StatList[d.M.stat] << "\t";
    os << "icode: " << d.M.icode << "\t";
    os << "valE: 0x" << std::hex << d.M.valE << "\t" << "valA: 0x" <<  d.M.valA << "\t";
    os << "dstE: " << RegList[d.M.dstE] << "\t" << "detM: " << RegList[d.M.dstM] << "\n\n";

    os << "W_Reg:\n" << "Stat: " << StatList[d.W.stat] << "\t";
    os << "icode: " << d.W.icode << "\t";
    os << "valE: 0x" << std::hex << d.W.valE << "\t" << "valM: 0x" <<  d.W.valM << "\t";
    os << "dstE: " << RegList[d.W.dstE] << "\t" << "detM: " << RegList[d.W.dstM] << "\n\n";

    os << "\n--------------------------------------------------------------------------------------------------------\n";
}

