//
// Created by 王少文 on 2020/11/27.
//
#include "Device.h"

#include <stdexcept>
#include <string>

#include "instr.h"
#include "util.h"
Device::Device(std::string str) : CFLAG{true, false, false} {
    memset(Reg, 0, REG_SIZE);
    memset(Mem, 0, MEM_SIZE);
    Stat = SAOK;
    if (str.length() >= 2 * MEM_SIZE) {
        throw std::domain_error("String is too long to flash");
    }
    int i = 0;
    auto p = str.begin();
    while (p != str.end()) {
        Mem[i] = 16 * CharToUint8(*p++) + CharToUint8(*p++);
        i++;
    }
}
bool Device::IfAddrValid(uint64_t pos) { return pos >= 0 && pos < MEM_SIZE; }
bool Device::IfInstrValid(uint8_t icode) { return icode >= 0 && icode <= 0xe; }
uint8_t Device::ReadHigh4Bits(uint64_t pos) const {
    return (Mem[pos] & 0xf0u) >> 4u;
}
uint8_t Device::ReadLow4Bits(uint64_t pos) const { return (Mem[pos] & 0x0fu); }
uint64_t Device::Read8Bytes(uint64_t pos) const {
    return *(uint64_t *)(Mem + pos);
}
void Device::Write8Bytes(uint64_t pos, uint64_t val) {
    *(uint64_t *)(Mem + pos) = val;
}
void Device::Fetch() {
    if (F.control == CSTALL) {
        return;
    }
    uint64_t f_pc = SelectPC();
    f.icode = ReadHigh4Bits(f_pc);
    f.ifun = ReadLow4Bits(f_pc);
    f.Stat = SelectFStat();
    bool need_regids =
        In(f.icode, IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ);
    bool need_valC = In(f.icode, IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL);
    if (need_regids) {
        f.ra = ReadHigh4Bits(f_pc + 1);
        f.rb = ReadLow4Bits(f_pc + 1);
    }
    if (need_valC) {
        f.valC = Read8Bytes(f_pc + need_regids + 1);
    }
    f.valP = f_pc + 1 + need_regids + 8 * need_valC;
    if (In(SHLT, f.Stat)) {
        F.predPC = f_pc;
    } else {
        SetFPredPc();
    }
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
uint64_t Device::SelectPC() const {
    if (M.icode == IJXX && !M.Cnd) {
        return M.valA;  // JXX但是没有成功跳转的情况
    } else if (W.icode == IRET) {
        return W.valM;  // Ret的情况，后期使用硬件栈时可以删去这一句话
    } else {
        return F.predPC;
    }
}
void Device::SetFPredPc() {
    if (In(f.icode, IJXX, ICALL)) {
        F.predPC = f.valC;
    } else {
        F.predPC = f.valP;
    }
}
void Device::F2D() {
    D.stat = f.Stat;
    D.icode = f.icode;
    D.ifun = f.ifun;
    D.rA = f.ra;
    D.rB = f.rb;
    D.valC = f.valC;
    D.valP = f.valP;
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
    uint64_t rvalA = d.srcA != RNONE ? Reg[d.srcA] : 0;
    uint64_t rvalB = d.srcB != RNONE ? Reg[d.srcB] : 0;
    //写valA，下面主要在处理forwarding的情况
    d.valA = rvalA;
    //写valB，下面主要在处理forwarding的情况
    d.valB = rvalB;
    d.stat = D.stat;
    d.icode = D.icode;
    d.ifun = D.ifun;
    d.valC = D.valC;
}
uint8_t Device::SelectDstM() const {
    if (In(D.icode, IMRMOVQ, IPOPQ)) {
        return D.rA;
    } else {
        return RNONE;
    }
}
uint8_t Device::SelectDstE() const {
    if (In(D.icode, IRRMOVQ, IIRMOVQ, IOPQ)) {
        return D.rB;
    } else if (In(D.icode, IPUSHQ, IPOPQ, ICALL, IRET)) {
        return RRSP;
    } else {
        return RNONE;
    }
}
uint8_t Device::SelectSrcB() const {
    if (In(D.icode, IOPQ, IRMMOVQ, IMRMOVQ)) {
        return D.rB;
    } else if (In(D.icode, IPUSHQ, IPOPQ, ICALL, IRET)) {
        return RRSP;
    } else {
        return RNONE;
    }
}
uint8_t Device::SelectSrcA() const {
    if (In(D.icode, IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ)) {
        return D.rA;
    } else if (In(D.icode, IPOPQ, IRET)) {
        return RRSP;
    } else {
        return RNONE;
    }
}
void Device::Execute() {
    if (E.control == CBUBBLE) {
        E.reset();
    }
    // 设置aluA
    uint64_t aluA = SelectAluA();
    // 设置aluB
    uint64_t aluB = SelectAluB();
    // 设置alufun
    uint8_t alufun = E.icode == IOPQ ? E.ifun : ALUADD;
    // 设置set_cc
    bool set_cc = In(E.icode, IOPQ) && !In(m.stat, SADR, SINS, SHLT) &&
                  !In(W.stat, SADR, SINS, SHLT);
    // ALU计算
    auto func = GetALUFunc(alufun);
    e.valE = func(aluA, aluB);
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
    e.valA = E.valA;
    e.dstM = E.dstM;
}
uint64_t Device::SelectAluB() const {
    if (In(E.icode, IRMMOVQ, IMRMOVQ, IOPQ, ICALL, IPUSHQ, IRET, IPOPQ)) {
        return E.valB;
    } else if (In(E.icode, IRRMOVQ, IIRMOVQ)) {
        return 0;
    } else {
        return 0xdeadbeef;
    }
}
uint64_t Device::SelectAluA() const {
    if (In(E.icode, IRRMOVQ, IOPQ)) {
        return E.valA;
    } else if (In(E.icode, IIRMOVQ, IRMMOVQ, IMRMOVQ)) {
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
}
std::function<uint64_t(uint64_t, uint64_t)> Device::GetALUFunc(uint8_t ifun) {
    switch (ifun) {
        case ALUADD:
            return [](uint64_t a, uint64_t b) { return a + b; };
        case ALUSUB:
            return [](uint64_t a, uint64_t b) { return b - a; };
        case ALUAND:
            return [](uint64_t a, uint64_t b) { return a & b; };
        case ALUXOR:
            return [](uint64_t a, uint64_t b) { return a ^ b; };
        default:
            // 若ALU指令不已知，设置错误码并返回返回0的函数
            E.stat = SINS;
            return [](uint64_t a, uint64_t b) { return 0; };
    }
}
bool Device::CalcCond(bool cflag[3]) const {
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
            return false;
    }
}
void Device::E2M() {
    M.stat = e.stat;
    M.icode = e.icode;
    M.Cnd = e.Cnd;
    M.valE = e.valE;
    M.valA = e.valA;
    M.dstE = e.dstE;
    M.dstM = e.dstM;
}
void Device::Memory() {
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
        if (IfAddrValid(mem_addr)) {
            m.valM = Read8Bytes(mem_addr);
        } else {
            m.stat = SADR;
            if (In(M.icode, IPUSHQ, IPOPQ)) {
                M.valE = 0;
            }
        }
    } else if (mem_write) {
        if (IfAddrValid(mem_addr)) {
            Write8Bytes(mem_addr, M.valA);
        } else {
            m.stat = SADR;
            if (In(M.icode, IPUSHQ, IPOPQ)) {
                M.valE = 0;
            }
        }
    }
    m.icode = M.icode;
    m.valE = M.valE;
    m.dstE = M.dstE;
    m.dstM = M.dstM;
}
void Device::M2W() {
    W.stat = m.stat;
    W.icode = m.icode;
    W.valE = m.valE;
    W.valM = m.valM;
    W.dstE = m.dstE;
    W.dstM = m.dstM;
}
void Device::Writeback() {
    //写回寄存器
    if (W.dstE != RNONE) {
        Reg[W.dstE] = W.valE;
    }
    if (W.dstM != RNONE) {
        Reg[W.dstM] = W.valM;
    }
    //设置状态码
    if (W.stat == SBUB) {
        Stat = SAOK;
    } else {
        Stat = W.stat;
    }
}
bool Device::IfLoadUseH() const {
    return In(E.icode, IMRMOVQ, IPOPQ) && In(E.dstM, d.srcA, d.srcB);
}
bool Device::IfMispredicted() const { return E.icode == IJXX && !e.Cnd; }
bool Device::IfRet() const { return In(IRET, D.icode, E.icode); }
void Device::SetFControl() {
    if (!IfMispredicted() && (IfLoadUseH() || IfRet())) {
        F.control = CSTALL;
    } else {
        F.control = CNORMAL;
    }
}
void Device::SetDControl() {
    if (IfLoadUseH()) {
        D.control = CSTALL;
    } else if (IfRet() || IfMispredicted() || M.icode == IRET) {
        D.control = CBUBBLE;
    } else {
        D.control = CNORMAL;
    }
}
void Device::SetEControl() {
    if (IfMispredicted() || IfLoadUseH() || E.icode == IRET) {
        E.control = CBUBBLE;
    } else {
        E.control = CNORMAL;
    }
}
uint64_t Device::GetPC() const { return F.predPC + 1; }
void Device::SetDSrcA() {
    if (In(D.icode, ICALL, IJXX)) {
        d.valA = D.valP;
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
    if (d.srcB == e.dstE) {
        d.valB = e.valE;
    } else if (d.srcB == M.dstM) {
        d.valB = m.valM;
    } else if (d.srcB == M.dstE) {
        d.valB = M.valE;
    } else if (d.srcB == W.dstM) {
        d.valB = W.valM;
    } else if (d.srcB == W.dstE) {
        d.valB = W.valE;
    }
}
void Device::SetCC() {
    bool set_cc = In(E.icode, IOPQ) && !In(m.stat, SADR, SINS, SHLT) &&
                  !In(W.stat, SADR, SINS, SHLT);
    if (set_cc) {
        for (int i = 0; i < 3; i++) {
            CFLAG[i] = e.CFLAG[i];
        }
    }
}
