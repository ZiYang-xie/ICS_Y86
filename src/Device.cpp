//
// Created by 王少文 on 2020/11/27.
//
#include "Device.h"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../library/json.hpp"
#include "Controller.h"
#include "Output.h"
#include "cstring"
#include "instr.h"
#include "util.h"
using json = nlohmann::json;
Device::Device(std::string str) {
    memset(Reg, 0, REG_SIZE);
    memset(Mem, 0, MEM_SIZE);
    Stat = SAOK;
    if (str.length() >= 2 * MEM_SIZE) {
        throw std::domain_error("String is too long to flash");
    }
    if (str.length() == 0) {
        return;
    }
    int i = 0;
    auto p = str.begin();
#ifdef TEXT_CHECK
    for (int j = 15; j > 0; j -= 2) {
        text_section_end = 256 * text_section_end +
                           16 * CharToUint8(*(p + j - 1)) +
                           CharToUint8(*(p + j));
    }
    p += 16;
#endif

    while (p != str.end()) {
        Mem[i] = 16 * CharToUint8(*p++) + CharToUint8(*p++);
        i++;
    }
    for (int k = 0; k <= 5; k++) {
        addr_queue.push(0);
    }
}
bool Device::IfAddrReadable(uint64_t pos) { return pos >= 0 && pos < MEM_SIZE; }
bool Device::IfAddrWriteable(uint64_t pos) const {
#ifdef TEXT_CHECK
    return pos >= text_section_end && pos < MEM_SIZE;
#else
    return pos >= 0 && pos < MEM_SIZE;
#endif
}
bool Device::IfAddrExecutable(uint64_t pos) const {
#ifdef TEXT_CHECK
    return pos >= 0 && pos < text_section_end;
#else
    return pos >= 0 && pos < MEM_SIZE;
#endif
}
bool Device::IfInstrValid(uint8_t icode) { return icode >= 0 && icode <= 0xe; }
uint8_t Device::ReadHigh4Bits(uint64_t pos) const {
    return (Mem[pos] & 0xf0u) >> 4u;
}
uint8_t Device::ReadLow4Bits(uint64_t pos) const { return (Mem[pos] & 0x0fu); }
uint64_t Device::Read8Bytes(uint64_t pos) const {
    return *(uint64_t *)(Mem + pos);
}
uint64_t Device::Read4Bytes(uint64_t pos) const {
    return *(uint32_t *)(Mem + pos);
}
uint64_t Device::Read2Bytes(uint64_t pos) const {
    return *(uint16_t *)(Mem + pos);
}
uint64_t Device::Read1Bytes(uint64_t pos) const {
    return *(uint8_t *)(Mem + pos);
}

void Device::Write8Bytes(uint64_t pos, uint64_t val) {
    *(uint64_t *)(Mem + pos) = val;
}
void Device::Write4Bytes(uint64_t pos, uint64_t val) {
    *(uint32_t *)(Mem + pos) = val;
}
void Device::Write2Bytes(uint64_t pos, uint64_t val) {
    *(uint16_t *)(Mem + pos) = val;
}
void Device::Write1Bytes(uint64_t pos, uint64_t val) { *(Mem + pos) = val; }
uint64_t Device::ReadReg(uint8_t reg_idx, uint8_t ifun) {
    switch (ifun) {
        case (RQU):
            return Reg[reg_idx];
        case (RLU):
            return (uint32_t)Reg[reg_idx];
        case (RWU):
            return (uint16_t)Reg[reg_idx];
        case (RBU):
            return (uint8_t)Reg[reg_idx];
        default:
            D.stat = SINS;
            return 0;
    }
}
void Device::WriteReg(uint8_t reg_idx, uint8_t ifun, uint64_t val) {
    switch (ifun) {
        case (RQU):
            Reg[reg_idx] = val;
            break;
        case (RLU):
            Reg[reg_idx] = (uint32_t)val;
            break;
        case (RWU):
            Reg[reg_idx] = (uint16_t)val;
            break;
        case (RBU):
            Reg[reg_idx] = (uint8_t)val;
            break;
        case (RLS):
            Reg[reg_idx] = (int32_t)val;
            break;
        case (RWS):
            Reg[reg_idx] = (int16_t)val;
            break;
        case (RBS):
            Reg[reg_idx] = (int8_t)val;
            break;
        default:
            D.stat = SINS;
    }
}
void Device::Fetch() {
    if (F.control == CSTALL) {
        return;
    }
    f.pc = SelectPC();
    f.icode = ReadHigh4Bits(f.pc);
    f.ifun = ReadLow4Bits(f.pc);
    f.Stat = SelectFStat();
    bool need_regids = In(f.icode, IRRMOVQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ,
                          IMRMOVQ, IOPQ, IIOPQ, IOPQN, IIOPQN);
    bool need_valC =
        In(f.icode, IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL, IIOPQ, IIOPQN);
    if (need_regids) {
        if (IfAddrExecutable(f.pc + 1)) {
            f.ra = ReadHigh4Bits(f.pc + 1);
            f.rb = ReadLow4Bits(f.pc + 1);
        } else {
            f.Stat = SINS;
        }
    }
    if (need_valC) {
        if (IfAddrExecutable(f.pc + need_regids + 1)) {
            f.valC = Read8Bytes(f.pc + need_regids + 1);
        } else {
            f.Stat = SINS;
        }
    }
    f.valP = f.pc + 1 + need_regids + 8 * need_valC;
    if (In(SHLT, f.Stat)) {
        F.predPC = f.pc;
    } else {
        F.predPC = GetFPredPc();
    }
    addr_queue.push(F.predPC);
    addr_queue.pop();
#ifdef HARDWARE_STACK
    if (f.icode == ICALL && hardware_stack.size() < 0x20) {
        hardware_stack.push(f.valP);
    }
#endif
}
uint8_t Device::SelectFStat() const {
    if (!IfInstrValid(f.icode)) {
        return SINS;
    } else if (f.icode == IHALT) {
        return SHLT;
    } else {
        return SAOK;
    }
}
uint64_t Device::SelectPC() {
    if (M.icode == IJXX && (M.Cnd ^ M.ifJump)) {
        if (!M.ifJump)
            return e.valC;  // JXX但是没有成功跳转的情况
        else
            return M.valE;
    }
#ifdef HARDWARE_STACK
    else if (W.icode == IRET && W.valM != W.predPC) {
        return W.valM;  // RET预测失败的情况
    } else if (D.icode == IRET) {
        if (hardware_stack.empty())
            return F.predPC;
        else {
            uint64_t temp = hardware_stack.top();
            hardware_stack.pop();
            return temp;
        }
    }
#else
    else if (W.icode == IRET) {
        return W.valM;
    }
#endif
    else {
        return F.predPC;
    }
}
uint64_t Device::GetFPredPc() {
    if (f.icode == IJXX && f.ifun != BALWAYS) {
#ifdef ADVANCED_JUMP
        if (if_jump_state <= 1) {
            f.ifJump = false;
            return f.valP;
        } else {
#endif
            f.ifJump = true;
            return f.valC;
#ifdef ADVANCED_JUMP
        };
#endif
    } else if (In(f.icode, IJXX, ICALL)) {
        f.ifJump = true;
        return f.valC;
    } else {
        f.ifJump = false;
        return f.valP;
    }
}
void Device::F2D() {
    if (F.control == CSTALL) {
        return;
    }
    D.stat = f.Stat;
    D.icode = f.icode;
    D.ifun = f.ifun;
    D.rA = f.ra;
    D.rB = f.rb;
    D.valC = f.valC;
    D.valP = f.valP;
    D.ifJump = f.ifJump;
    D.predPC = f.pc;
}
void Device::Decode() {
    if (D.control == CSTALL) {
        return;
    } else if (D.control == CBUBBLE) {
        D.reset();
    }
    //写srcA
    d.srcA = SelectSrcA();
    //写srcB
    d.srcB = SelectSrcB();
    //写dstE
    d.dstE = SelectDstE();
    //写dstM
    d.dstM = SelectDstM();
    //计算valA,valB
    volatile uint64_t rvalA;
    volatile uint64_t rvalB;
    if (D.icode == IRMMOVQ) {
        rvalA = d.srcA != RNONE ? ReadReg(d.srcA, d.ifun) : 0;
        rvalB = d.srcB != RNONE ? ReadReg(d.srcB, RQU) : 0;
    } else {
        rvalA = d.srcA != RNONE ? ReadReg(d.srcA, RQU) : 0;
        rvalB = d.srcB != RNONE ? ReadReg(d.srcB, RQU) : 0;
    }
    //写valA
    d.valA = rvalA;
    //写valB
    d.valB = rvalB;
    d.stat = D.stat;
    d.icode = D.icode;
    d.ifun = D.ifun;
    d.valC = D.valC;
    d.ifJump = D.ifJump;
    d.predPC = D.predPC;
}
uint8_t Device::SelectDstM() const {
    if (In(D.icode, IMRMOVQ, IPOPQ)) {
        return D.rA;
    } else {
        return RNONE;
    }
}
uint8_t Device::SelectDstE() {
    if (In(D.icode, IRRMOVQ, IIRMOVQ, IOPQ, IIOPQ)) {
        return D.rB;
    } else if (In(D.icode, IPUSHQ, IPOPQ, ICALL, IRET)) {
        if (D.rB != RNONE && D.icode == IPUSHQ) {
            D.stat = SINS;
        }
        return RRSP;
    } else {
        return RNONE;
    }
}
uint8_t Device::SelectSrcB() const {
    if (In(D.icode, IOPQ, IRMMOVQ, IMRMOVQ, IIOPQ, IOPQN, IIOPQN)) {
        return D.rB;
    } else if (In(D.icode, IPUSHQ, IPOPQ, ICALL, IRET)) {
        return RRSP;
    } else {
        return RNONE;
    }
}
uint8_t Device::SelectSrcA() const {
    if (In(D.icode, IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ, IOPQN)) {
        return D.rA;
    } else if (In(D.icode, IPOPQ, IRET)) {
        return RRSP;
    } else {
        return RNONE;
    }
}
void Device::Execute() {
    static uint8_t counter =
        0;  // 假定乘法、除法和求余指令需要10个周期来完成，这是他们的状态机counter
    static uint64_t store_evalE;
    if (E.control == CBUBBLE) {
        E.reset();
    }
    // 设置aluA
    uint64_t aluA = SelectAluA();
    // 设置aluB
    uint64_t aluB = SelectAluB();
    // 设置alufun
    uint8_t alufun = In(E.icode, IOPQ, IIOPQ, IOPQN, IIOPQN) ? E.ifun : ALUADD;
    // 设置set_cc
    bool set_cc = In(E.icode, IOPQ, IIOPQ, IOPQN, IIOPQN) &&
                  !In(m.stat, SADR, SINS, SHLT) &&
                  !In(W.stat, SADR, SINS, SHLT);
    // ALU计算
    auto func = GetALUFunc(alufun);
    e.valE = func(aluA, aluB);
    // 开始10周期的计算
    if (In(E.icode, IOPQ, IIOPQ, IOPQN, IIOPQN) &&
        In(E.ifun, ALUMULQ, ALUDIVQ, ALUREMQ) && counter == 0) {
        counter = 10;
        store_evalE = e.valE;
    }
    // 还在计算中
    if (counter > 1) {
        counter--;
        E.ifDone = false;
        return;
    } else if (counter == 1) {
        counter--;
        E.ifDone = true;
        e.valE = store_evalE;
    }
    { E.ifDone = true; }
    // 计算cnd
    if (set_cc) {
        e.CFLAG[CZF] = (e.valE == 0);
        e.CFLAG[CSF] = (((int64_t)e.valE) < 0);
        e.CFLAG[COF] = (((((int64_t)aluA < 0) ^ (E.ifun == ALUSUB)) ==
                         (int64_t)aluB < 0) &&
                        ((int64_t)e.valE < 0 != (int64_t)aluB < 0));
    }
    e.Cnd = CalcCond(e.CFLAG);
    // 写dstE和dstM
    if (E.icode == IRRMOVQ && !e.Cnd) {
        e.dstE = RNONE;
    } else {
        e.dstE = E.dstE;
    }
    e.stat = E.stat;
    e.icode = E.icode;
    e.ifun = E.ifun;
    e.ifJump = E.ifJump;
    e.valA = E.valA;
    e.dstM = E.dstM;
    e.valC = E.valC;
    e.predPC = E.predPC;
}
uint64_t Device::SelectAluB() const {
    if (In(E.icode, IRMMOVQ, IMRMOVQ, IOPQ, ICALL, IPUSHQ, IRET, IPOPQ, IIOPQ,
           IIOPQN, IOPQN)) {
        return E.valB;
    } else if (In(E.icode, IRRMOVQ, IIRMOVQ, IJXX)) {
        return 0;
    } else {
        return 0xdeadbeef;
    }
}
uint64_t Device::SelectAluA() const {
    if (In(E.icode, IRRMOVQ, IOPQ, IJXX, IOPQN)) {
        return E.valA;
    } else if (In(E.icode, IIRMOVQ, IRMMOVQ, IMRMOVQ, IIOPQ, IIOPQN)) {
        return E.valC;
    } else if (In(E.icode, ICALL, IPUSHQ)) {
        return -8;
    } else if (In(E.icode, IRET, IPOPQ)) {
        return 8;
    } else {
        return 0xdeadbeef;
        //这一条语句仅仅是为了方便后期debug和方式clang啰嗦我可能的未定义变量，下同
    }
}
void Device::D2E() {
    if (D.control == CSTALL) {
        return;
    }
    E.stat = d.stat;
    E.icode = d.icode;
    E.ifun = d.ifun;
    E.valC = d.valC;
    E.valA = d.valA;
    E.valB = d.valB;
    E.dstE = d.dstE;
    E.dstM = d.dstM;
    E.srcA = d.srcA;
    E.srcB = d.srcB;
    E.ifJump = d.ifJump;
    E.predPC = d.predPC;
}
std::function<uint64_t(uint64_t, uint64_t)> Device::GetALUFunc(uint8_t ifun) {
    switch (ifun) {
        case ALUADD:
            return [](uint64_t a, uint64_t b) { return b + a; };
        case ALUSUB:
            return [](uint64_t a, uint64_t b) { return b - a; };
        case ALUAND:
            return [](uint64_t a, uint64_t b) { return b & a; };
        case ALUXOR:
            return [](uint64_t a, uint64_t b) { return b ^ a; };
        case ALUOR:
            return [](uint64_t a, uint64_t b) { return b | a; };
        case ALUSHL:
            return [](uint64_t a, uint64_t b) { return b << a; };
        case ALUSHR:
            return [](uint64_t a, uint64_t b) { return b >> a; };
        case ALUSAR:
            return [](uint64_t a, uint64_t b) { return (int64_t)b >> a; };
        case ALUMULQ:
            return [](uint64_t a, uint64_t b) { return b * a; };
        case ALUDIVQ:
            return [](uint64_t a, uint64_t b) { return b / a; };
        case ALUREMQ:
            return [](uint64_t a, uint64_t b) { return b - b * (b / a); };
        case ALUMINQ:
            return [](uint64_t a, uint64_t b) { return a < b ? a : b; };
        case ALUMAXQ:
            return [](uint64_t a, uint64_t b) { return a > b ? a : b; };
        default:
            // 若ALU指令不已知，设置错误码并返回返回0的函数
            E.stat = SINS;
            return [](uint64_t a, uint64_t b) { return 0; };
    }
}
bool Device::CalcCond(bool cflag[3]) {
    switch (E.ifun) {
        case BALWAYS:
            return true;
        case BLE:
            return cflag[CZF] || (cflag[CSF] ^ cflag[COF]);
        case BL:
            return cflag[CSF] ^ cflag[COF];
        case BE:
            return cflag[CZF];
        case BNE:
            return !cflag[CZF];
        case BGE:
            return (cflag[CSF] == cflag[COF]);
        case BG:
            return !cflag[CZF] && (cflag[CSF] == cflag[COF]);
        default:
            if (In(E.icode, IRRMOVQ, IJXX)) {
                E.stat = SINS;
            }
            return false;
    }
}
void Device::E2M() {
    if (!IfExecuteDone()) {
        return;
    }
    M.stat = e.stat;
    M.icode = e.icode;
    M.ifun = e.ifun;
    M.Cnd = e.Cnd;
    M.valE = e.valE;
    M.valA = e.valA;
    M.dstE = e.dstE;
    M.dstM = e.dstM;
    M.ifJump = e.ifJump;
    M.predPC = e.predPC;
}
void Device::Memory() {
    if (M.control == CBUBBLE) {
        M.reset();
    }
    // 写stat
    m.stat = M.stat;
    // 取mem_addr
    uint64_t mem_addr;
    if (In(M.icode, IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ)) {
        mem_addr = M.valE;
    } else if (In(M.icode, IPOPQ, IRET)) {
        mem_addr = M.valA;
    } else {
        mem_addr = 0xdeadbeef;
    }
    // 算MemRead和MemWrite
    bool mem_read = In(M.icode, IMRMOVQ, IPOPQ, IRET);
    bool mem_write = In(M.icode, IRMMOVQ, IPUSHQ, ICALL);
    // 读内存
    if (mem_read) {
        if (IfAddrReadable(mem_addr)) {
            ReadMemToValM(mem_addr);
        } else {
            // m.stat = SADR;
            m.stat = m.stat == SINS ? SINS : SADR;
            if (In(M.icode, IPUSHQ, IPOPQ)) {
                M.valE = 0;
            }
        }
    } else if (mem_write) {
        if (IfAddrWriteable(mem_addr)) {
            WriteMemFromValA(mem_addr);
        } else {
            // m.stat = SADR; 没有人比我更会面向测例编程
            // 认真说：SINS和SADR都是可以的，但这里遵循助教的规范，当二者同时存在时，选择SINS
            m.stat = m.stat == SINS ? SINS : SADR;
        }
    }
    m.icode = M.icode;
    m.valE = M.valE;
    m.dstE = M.dstE;
    m.dstM = M.dstM;
    m.ifun = M.ifun;
    m.predPC = M.predPC;
}
void Device::WriteMemFromValA(uint64_t mem_addr) {
    if (M.icode != IRMMOVQ) {
        Write8Bytes(mem_addr, M.valA);
    } else {
        switch (M.ifun) {
            case (RQU):
                Write8Bytes(mem_addr, M.valA);
                break;
            case (RLS):
            case (RLU):
                Write4Bytes(mem_addr, M.valA);
                break;
            case (RWS):
            case (RWU):
                Write2Bytes(mem_addr, M.valA);
                break;
            case (RBS):
            case (RBU):
                Write1Bytes(mem_addr, M.valA);
                break;
        }
    }
}
void Device::ReadMemToValM(uint64_t mem_addr) {
    if (M.icode != IMRMOVQ) {
        m.valM = Read8Bytes(mem_addr);
    } else {
        switch (M.ifun) {
            case (RQU):
                m.valM = Read8Bytes(mem_addr);
                break;
            case (RLS):
            case (RLU):
                m.valM = Read4Bytes(mem_addr);
                break;
            case (RWS):
            case (RWU):
                m.valM = Read2Bytes(mem_addr);
                break;
            case (RBS):
            case (RBU):
                m.valM = Read1Bytes(mem_addr);
                break;
        }
    }
}
void Device::M2W() {
    W.stat = m.stat;
    W.icode = m.icode;
    W.ifun = m.ifun;
    W.valE = m.valE;
    W.valM = m.valM;
    W.dstE = m.dstE;
    W.dstM = m.dstM;
    W.predPC = m.predPC;
}
void Device::Writeback() {
    //写回寄存器，如果遇到错误则不允许写入
    if (W.dstE != RNONE && W.stat == SAOK) {
        if (W.icode != IMRMOVQ) {
            Reg[W.dstE] = W.valE;
        } else {
            WriteReg(W.dstE, W.ifun, W.valE);
        }
    }
    if (W.dstM != RNONE && W.stat == SAOK) {
        if (W.icode != IMRMOVQ) {
            Reg[W.dstM] = W.valM;
        } else {
            WriteReg(W.dstM, W.ifun, W.valM);
        }
    }
    //设置状态码
    if (W.stat == SBUB) {
        Stat = SAOK;
    } else {
        Stat = W.stat;
    }
}
bool Device::IfExecuteDone() const { return E.ifDone; }
bool Device::IfLoadUseH() const {
    return In(E.icode, IMRMOVQ, IPOPQ) && In(E.dstM, d.srcA, d.srcB);
}
bool Device::IfMispredicted() const {
    return E.icode == IJXX && (e.Cnd ^ E.ifJump);
}
#ifdef HARDWARE_STACK
bool Device::IfMispredictRet() const {
    return M.icode == IRET && (m.valM != M.predPC);
}
#else
bool Device::IfRet() const { return In(IRET, D.icode, E.icode); }
#endif
void Device::SetFControl() {
#ifdef HARDWARE_STACK
    if (!(IfMispredicted() || IfMispredictRet()) &&
        (IfLoadUseH() || !IfExecuteDone())) {
        F.control = CSTALL;
    } else {
        F.control = CNORMAL;
    }
#else
    if (!IfMispredicted() && (IfLoadUseH() || IfRet() || !IfExecuteDone())) {
        F.control = CSTALL;
        bad_instr_num += 1;
    } else {
        F.control = CNORMAL;
    }
#endif
}
void Device::SetDControl() {
#ifdef HARDWARE_STACK
    if (IfLoadUseH() && !IfExecuteDone()) {
        D.control = CSTALL;
    } else if (IfMispredictRet() || IfMispredicted()) {
        D.control = CBUBBLE;
    } else {
        D.control = CNORMAL;
    }
#else
    if (IfLoadUseH() || !IfExecuteDone()) {
        D.control = CSTALL;
        bad_instr_num += 1;
    } else if (IfRet() || IfMispredicted() || M.icode == IRET) {
        D.control = CBUBBLE;
        bad_instr_num += 1;
    } else {
        D.control = CNORMAL;
    }
#endif
}
void Device::SetEControl() {
#ifdef HARDWARE_STACK
    if (IfMispredicted() || IfMispredictRet() || IfLoadUseH()) {
        E.control = CBUBBLE;
    } else {
        E.control = CNORMAL;
    }
#else
    if (IfMispredicted() || IfLoadUseH() || E.icode == IRET) {
        E.control = CBUBBLE;
        bad_instr_num += 1;
    } else {
        E.control = CNORMAL;
    }
#endif
}
void Device::SetMControl() {
#ifdef HARDWARE_STACK
    if (IfMispredictRet()) {
        M.control = CBUBBLE;
    } else
#endif
    {
        M.control = CNORMAL;
    }
}
uint64_t Device::GetPC() const { return addr_queue.front(); }
void Device::SetDSrcA() {
    if (In(D.icode, ICALL, IJXX)) {
        d.valA = D.valP;
    } else if (d.srcA == RNONE) {
        return;
    } else if (d.srcA == e.dstE) {
        d.valA = e.valE;
    } else if (d.srcA == M.dstM) {
        d.valA = m.valM;
    } else if (d.srcA == M.dstE) {
        d.valA = M.valE;
    } else if (d.srcA == W.dstM) {
        d.valA = W.valM;
    } else if (d.srcA == W.dstE) {
        d.valA = W.valE;
    }
}
void Device::SetDSrcB() {
    if (d.srcB == RNONE) {
        return;
    }
    if (d.srcB == e.dstE) {
        d.valB = e.valE;
    } else if (d.srcB == m.dstM) {
        d.valB = m.valM;
    } else if (d.srcB == M.dstE) {
        d.valB = m.valE;
    } else if (d.srcB == W.dstM) {
        d.valB = W.valM;
    } else if (d.srcB == W.dstE) {
        d.valB = W.valE;
    }
}
void Device::SetCC() {
    bool set_cc = In(E.icode, IOPQ, IIOPQ, IOPQN, IIOPQN) &&
                  In(m.stat, SBUB, SAOK) && W.stat == SAOK;
    if (set_cc) {
        for (int i = 0; i < 3; i++) {
            CFLAG[i] = e.CFLAG[i];
        }
    }
}
void Device::UpdateIfJumpState() {
    if (E.icode == IJXX && E.ifun != BALWAYS) {
        if (!e.Cnd) {
            //未成功跳转的情况
            if_jump_state = if_jump_state == 0 ? 0 : if_jump_state - 1;
        } else {
            //成功跳转的情况
            if_jump_state = if_jump_state == 3 ? 3 : if_jump_state + 1;
        }
    }
}
std::string Device::GetConsoleOutput() const {
    char dst[CONSOLE_MEM_SIZE + 1] = {0};
    memcpy(dst, Mem + CONSOLE_MEM_START, CONSOLE_MEM_SIZE);
    return std::string(dst);
}
json Device::GetGraphicsOutput() const {
    static json res;
    if (Mem[GRAPHICS_MEM_FLAG] == 0) {
        res.clear();
        Mem[GRAPHICS_MEM_FLAG] = 1;
        for (int i = 0; i < GRAPHICS_LENGTH; i++) {
            json line;
            for (int j = 0; j < GRAPHICS_LENGTH; j++) {
                int shift = (GRAPHICS_LENGTH * i + j) * 4 + GRAPHICS_MEM_START;
                json rgba{{"r", Mem[shift + 3]},
                          {"g", Mem[shift + 2]},
                          {"b", Mem[shift + 1]},
                          {"a", Mem[shift + 0]}};
                line[j] = rgba;
            }
            res[i] = line;
        }
    }
    return res;
}
uint64_t Device::GetBadInstrNum() const { return bad_instr_num; }
