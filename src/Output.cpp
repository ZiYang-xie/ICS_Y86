//
// Created by 王少文 on 2020/12/12.
//
#include "Output.h"

template <typename T, unsigned long N>
static std::string GetName(std::array<T, N> arr, int idx) {
    if (idx >= 0 && idx < N) {
        return arr[idx];
    } else {
        std::cerr << "output.GetName, index out of bounds";
        return "";
    }
}
std::string GetStatName(int idx) {
    const std::array<std::string, 6> StatList = {"None", "AOK", "HLT",
                                                 "ADR",  "INS", "BUB"};
    return GetName(StatList, idx);
}
std::string GetCCName(int idx) {
    const std::array<std::string, 3> CCList = {"ZF", "SF", "OF"};
    return GetName(CCList, idx);
}
std::string GetRegName(int idx) {
    const std::array<std::string, 16> RegList = {
        "RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI",
        "R8",  "R9",  "R10", "R11", "R12", "R13", "R14", "None"};
    return GetName(RegList, idx);
}
std::string GetControlName(int idx) {
    const std::array<std::string, 3> ControlList = {"Normal", "Bubble",
                                                    "Stall"};
    return GetName(ControlList, idx);
}
void OutputProcedure(std::ostream& os, int idx, const Device& d) {
    os << std::endl;
    os << "Cycle " << std::dec << idx++ << ": "
       << "\n\n";
    for (int i = 0; i < REG_SIZE; ++i) {
        os << GetRegName(i) << ":\t0x" << std::hex << d.Reg[i] << "  ";
        if (!(i & 1)) os << std::endl;
    }
    os << "\n";
    for (int i = 0; i < 3; i++) {
        os << GetCCName(i) << ":\t" << d.CFLAG[i] << "  ";
    }
    os << "\n\n";
    os << "F_Reg:\nControl: " << GetControlName(d.F.control) << "\t\t"
       << "predPC: 0x" << std::hex << d.F.predPC << "\n\n";

    os << "D_Reg:\nControl: " << GetControlName(d.D.control) << "\t\t"
       << "Stat: " << GetStatName(d.D.stat) << "\t";
    if (d.D.icode | d.D.ifun)
        os << "icode: " << std::hex << d.D.icode << "\t"
           << "ifun: " << d.D.ifun << "\t";
    os << "rA: " << GetRegName(d.D.rA) << "\t\t"
       << "rB: " << GetRegName(d.D.rB) << "\t\t";
    os << "valC: 0x" << std::hex << d.D.valC << "\t\t"
       << "valP: 0x" << d.D.valP << "\n\n";

    os << "E_Reg:\nControl: " << GetControlName(d.E.control) << "\t\t"
       << "Stat: " << GetStatName(d.E.stat) << "\t";
    if (d.E.icode | d.E.ifun)
        os << "icode: " << std::hex << d.E.icode << "\t"
           << "ifun: " << d.E.ifun << "\t";
    os << "valC: 0x" << std::hex << d.E.valC << "\t"
       << "valA: 0x" << d.E.valA << "\t"
       << "valB: 0x" << d.E.valB << "\t";
    os << "srcA: " << GetRegName(d.E.srcA) << "\t"
       << "srcB: " << GetRegName(d.E.srcB) << "\t"
       << "dstE: " << GetRegName(d.E.dstE) << "\t"
       << "detM: " << GetRegName(d.E.dstM) << "\n\n";

    os << "M_Reg:\n"
       << "Stat: " << GetStatName(d.M.stat) << "\t";
    os << "icode: " << d.M.icode << "\t";
    os << "valE: 0x" << std::hex << d.M.valE << "\t"
       << "valA: 0x" << d.M.valA << "\t";
    os << "dstE: " << GetRegName(d.M.dstE) << "\t"
       << "detM: " << GetRegName(d.M.dstM) << "\n\n";

    os << "W_Reg:\n"
       << "Stat: " << GetStatName(d.W.stat) << "\t";
    os << "icode: " << d.W.icode << "\t";
    os << "valE: 0x" << std::hex << d.W.valE << "\t"
       << "valM: 0x" << d.W.valM << "\t";
    os << "dstE: " << GetRegName(d.W.dstE) << "\t"
       << "detM: " << GetRegName(d.W.dstM) << "\n\n";

    os << "\n------------------------------------------------------------------"
          "--------------------------------------\n";
}

void OutputFinal(const Device& d) {
    std::cout << std::endl;
    std::cout << "PC:\t0x" << std::hex << d.GetPC() << std::endl;
    std::cout << "State:\t" << GetStatName(d.Stat) << std::endl;

    std::cout << "CC:\t";
    for (int i = 0; i < 3; ++i)
        std::cout << GetCCName(i) << ": " << d.CFLAG[i] << "\t";

    std::cout << "\n\n";
    std::cout << "Register:" << std::endl;
    for (int i = 0; i < REG_SIZE; ++i) {
        std::cout << "\t" << GetRegName(i) << ":\t0x" << std::hex << d.Reg[i]
                  << '\t';
        if (!(i & 1)) std::cout << std::endl;
    }
}