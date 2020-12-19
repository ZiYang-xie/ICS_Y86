//
// Created by 王少文 on 2020/12/12.
//
#include "Output.h"

#include <map>

#include "Device.h"
#include "util.h"
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
        "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
        "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "None"};
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
    os << "PC " << std::hex << ": 0x" << d.GetPC() << "\n\n";
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
    os << "icode: " << std::hex << (int)d.D.icode << "\t"
       << "ifun: " << (int)d.D.ifun << "\t";
    os << "rA: " << GetRegName(d.D.rA) << "\t\t"
       << "rB: " << GetRegName(d.D.rB) << "\t\t";
    os << "valC: 0x" << std::hex << d.D.valC << "\t\t"
       << "valP: 0x" << d.D.valP << "\n\n";

    os << "E_Reg:\nControl: " << GetControlName(d.E.control) << "\t\t"
       << "Stat: " << GetStatName(d.E.stat) << "\t";
    os << "icode: " << std::hex << (int)d.E.icode << "\t"
       << "ifun: " << (int)d.E.ifun << "\t";
    os << "valC: 0x" << std::hex << d.E.valC << "\t"
       << "valA: 0x" << d.E.valA << "\t"
       << "valB: 0x" << d.E.valB << "\t";
    os << "srcA: " << GetRegName(d.E.srcA) << "\t"
       << "srcB: " << GetRegName(d.E.srcB) << "\t"
       << "dstE: " << GetRegName(d.E.dstE) << "\t"
       << "detM: " << GetRegName(d.E.dstM) << "\n\n";

    os << "M_Reg:\n"
       << "Stat: " << GetStatName(d.M.stat) << "\t";
    os << "icode: " << (int)d.M.icode << "\t";
    os << "valE: 0x" << std::hex << d.M.valE << "\t"
       << "valA: 0x" << d.M.valA << "\t";
    os << "dstE: " << GetRegName(d.M.dstE) << "\t"
       << "detM: " << GetRegName(d.M.dstM) << "\n\n";

    os << "W_Reg:\n"
       << "Stat: " << GetStatName(d.W.stat) << "\t";
    os << "icode: " << (int)d.W.icode << "\t";
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
#ifdef OUTPUT_JSON
json OutputToJsonCycle(const Device& d) {
    json res;
    res["State"] = GetStatName(d.Stat);
    res["PC"] = Format("0x%03x", d.GetPC());
    res["CC"] = json{{"zf", (int)d.CFLAG[0]},
                     {"sf", (int)d.CFLAG[1]},
                     {"of", (int)d.CFLAG[2]}};
    json reg;
    for (int i = 0; i < 15; i++) {
        reg[GetRegName(i)] = Format("0x%x", d.Reg[i]);
    }
    res["Register"] = reg;
    res["PipelineReg"] = {{"F_Reg", {{"predPC", Format("0x%x", d.F.predPC)}}},
                          {"D_Reg",
                           {
                               {"Stat", GetStatName(d.D.stat)},
                               {"icode", Format("%d", d.D.icode)},
                               {"ifun", Format("%d", d.D.ifun)},
                               {"rA", GetRegName(d.D.rA)},
                               {"rB", GetRegName(d.D.rA)},
                               {"valC", Format("0x%x", d.D.valC)},
                               {"valP", Format("0x%x", d.D.valP)},
                           }},
                          {"E_Reg",
                           {{"Stat", GetStatName(d.E.stat)},
                            {"icode", Format("%d", d.E.icode)},
                            {"ifun", Format("%d", d.E.ifun)},
                            {"valC", Format("0x%x", d.E.valC)},
                            {"valA", Format("0x%x", d.E.valA)},
                            {"valB", Format("0x%x", d.E.valB)},
                            {"srcA", GetRegName(d.E.srcA)},
                            {"srcB", GetRegName(d.E.srcB)},
                            {"dstE", GetRegName(d.E.srcB)},
                            {"detM", GetRegName(d.E.srcB)}}},
                          {"M_Reg",
                           {{"Stat", GetStatName(d.M.stat)},
                            {"icode", Format("%d", d.M.icode)},
                            {"valE", Format("0x%x", d.M.valE)},
                            {"valA", Format("0x%x", d.M.valA)},
                            {"dstE", GetRegName(d.M.dstE)},
                            {"detM", GetRegName(d.M.dstM)}}},
                          {"W_Reg",
                           {{"Stat", GetStatName(d.W.stat)},
                            {"icode", Format("%d", d.W.icode)},
                            {"valE", Format("0x%x", d.W.valE)},
                            {"valM", Format("0x%x", d.W.valM)},
                            {"dstE", GetRegName(d.W.dstE)},
                            {"detM", GetRegName(d.W.dstM)}}}};
    res["Console"] = d.GetConsoleOutput();
    res["Graphics"] = d.GetGraphicsOutput();
    return res;
}
void OutputToJsonFinal(uint64_t cycle_num, uint64_t ins_num,
                       const std::vector<json>& cycle, std::ostream& os) {
    json res;
    res["CycleNum"] = cycle_num;
    res["InsNum"] = ins_num;
    res["Cycle"] = json(cycle);
    os << "json_data(";
    os << res;
    os << ");";
}
#endif