//
// Created by 王少文 on 2020/11/27.
//
#include "Device.h"
#include "instr.h"
#include "util.h"
#include <stdexcept>
#include <string>
Device::Device(std::string str) {
  memset(Reg, 0, REG_SIZE);
  memset(CFLAG, 0, 3);
  memset(Mem, 0, MEM_SIZE);
  Stat = SAOK;
  if (str.length() >= MEM_SIZE - RESERVED_SIZE) {
    throw std::domain_error("String is too long to flash");
  }
  int i = 0;
  auto p = str.begin();
  while (p!=str.end()) {
    Mem[i] = 16*CharToUint8(*p++)+CharToUint8(*p++);
    i++;
  }
}
bool Device::IfAddrValid(uint64_t pos) {
  return pos >= 0 && pos <= MEM_SIZE - RESERVED_SIZE;
}
bool Device::IfInstrValid(uint8_t icode) { return icode >= 0 && icode <= 0xe; }
uint8_t Device::ReadHigh4Bits(uint64_t pos) const { return (Mem[pos] & 0xf0u)>>4u; }
uint8_t Device::ReadLow4Bits(uint64_t pos) const { return (Mem[pos] & 0x0fu) ; }
uint64_t Device::Read8Bytes(uint64_t pos) const {
  return *(uint64_t *)(Mem + pos);
}
void Device::Write8Bytes(uint64_t pos, uint64_t val) {
  *(uint64_t *)(Mem + pos) = val;
}
void Device::Fetch() {
  uint8_t c = GetFControl();
  if (c == CSTALL) {
    return;
  }
  uint8_t f_pc;
  if (M.icode == IJXX && !M.Cnd) {
    f_pc = M.valA; // JXX但是没有成功跳转的情况
  } else if (M.icode == IRET) {
    f_pc = W.valM; // Ret的情况，后期使用硬件栈时可以删去这一句话
  } else {
    f_pc = F.predPC;
  }
  f.icode = ReadHigh4Bits(f_pc);
  f.ifun = ReadLow4Bits(f_pc);
  if (!IfInstrValid(f.icode)) {
    f.Stat = SINS;
  } else if (f.icode == IHALT) {
    f.Stat = SHLT;
  } else{
      f.Stat = SAOK;
  }
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
  SetFPredPc();
  F2D();
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
  uint8_t c = GetDControl();
  if (c == CSTALL) {
    return;
  } else if (c == CBUBBLE) {
    D.reset();
    return;
  }
  //写srcA
  if (In(D.icode, IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ)) {
    d.srcA = D.rA;
  } else if (In(D.icode, IPOPQ, IRET)) {
    d.srcA = RRSP;
  } else {
    d.srcA = RNONE;
  }
  //写srcB
  if (In(D.icode, IOPQ, IRMMOVQ, IMRMOVQ)) {
    d.srcB = D.rB;
  } else if (In(D.icode, IPUSHQ, IPOPQ, ICALL, IRET)) {
    d.srcB = RRSP;
  } else {
    d.srcB = RNONE;
  }
  //写dstE
  if (In(D.icode, IRRMOVQ, IIRMOVQ, IOPQ)) {
    d.dstE = D.rB;
  } else if (In(D.icode, IPUSHQ, IPOPQ, ICALL, IRET)) {
    d.dstE = RRSP;
  } else {
    d.dstE = RNONE;
  }
  //写dstM
  if (In(D.icode, IMRMOVQ, IPOPQ)) {
    d.dstM = D.rA;
  } else {
    d.dstM = RNONE;
  }
  uint64_t rvalA = d.srcA != RNONE ? Reg[d.srcA] : 0;
  uint64_t rvalB = d.srcB != RNONE ? Reg[d.srcB] : 0;
  //写valA，下面主要在处理forwarding的情况
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
  } else {
    d.valA = rvalA;
  }
  //写valB，下面主要在处理forwarding的情况
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
  } else {
    d.valB = rvalB;
  }
  D2E();
}
void Device::Execute() {
  uint8_t c = GetEControl();
  if (c == CBUBBLE) {
    E.reset();
    return;
  }
  uint64_t aluA, aluB;
  // 设置aluA
  if (In(E.icode, IRRMOVQ, IOPQ)) {
    aluA = E.valA;
  } else if (In(E.icode, IIRMOVQ, IRMMOVQ, IMRMOVQ)) {
    aluA = E.valC;
  } else if (In(E.icode, ICALL, IPUSHQ)) {
    aluA = -8;
  } else if (In(E.icode, IRET, IPOPQ)) {
    aluA = 8;
  } else {
    aluA =
        0xdeadbeef; //这一条语句仅仅是为了方便后期debug和方式clang啰嗦我可能的未定义变量，下同
  }
  // 设置aluB
  if (In(E.icode, IRMMOVQ, IMRMOVQ, IOPQ, ICALL, IPUSHQ, IRET, IPOPQ)) {
    aluB = E.valB;
  } else if (In(E.icode, IRRMOVQ, IIRMOVQ)) {
    aluB = 0;
  } else {
    aluB = 0xdeadbeef;
  }
  // 设置alufun
  uint8_t alufun;
  if (E.icode == IOPQ) {
    alufun = E.ifun;
  } else {
    alufun = ALUADD;
  }
  // 设置set_cc
  bool set_cc = In(E.icode, IOPQ) && !In(m.stat, SADR, SINS, SHLT) &&
                !In(W.stat, SADR, SINS, SHLT);
  // ALU计算
  auto func = GetALUFunc(alufun);
  e.valE = func(aluA, aluB);
  // 计算cnd
  if (set_cc) {
    CFLAG[CZF] = (e.valE == 0);
    CFLAG[CSF] = (((int64_t)e.valE) < 0);
    CFLAG[COF] = (((int64_t)aluA < 0 == (int64_t)aluB < 0) &&
                  ((int64_t)e.valE < 0 != (int64_t)aluA < 0));
  }
  e.Cnd = cond();
  // 写dstE和dstM
  if (E.icode == IRRMOVQ && !e.Cnd) {
    e.dstE = RNONE;
  } else {
    e.dstE = E.dstE;
  }
  E2M();
}
void Device::D2E() {
  E.stat = D.stat;
  E.icode = D.icode;
  E.ifun = D.ifun;
  E.valC = D.valC;
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
    return [](uint64_t a, uint64_t b) { return a - b; };
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
bool Device::cond() const {
  switch (E.ifun) {
  case BALWAYS:
    return true;
  case BLE:
    return CFLAG[CZF] || (CFLAG[CSF] ^ CFLAG[COF]);
  case BL:
    return CFLAG[CSF] ^ CFLAG[COF];
  case BE:
    return CFLAG[CZF];
  case BNE:
    return !CFLAG[CZF];
  case BGE:
    return (CFLAG[CSF] == CFLAG[COF]);
  case BG:
    return !CFLAG[CZF] && (CFLAG[CSF] == CFLAG[COF]);
  default:
    return false;
  }
}
void Device::E2M() {
  M.stat = E.stat;
  M.icode = E.icode;
  M.Cnd = e.Cnd;
  M.valE = e.valE;
  M.valA = E.valA;
  M.dstE = e.dstE;
  M.dstM = E.dstM;
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
      m.valM = Read8Bytes(Mem[mem_addr]);
    } else {
      m.stat = SADR;
    }
  } else if (mem_write) {
    if (IfAddrValid(mem_addr)) {
      Write8Bytes(mem_addr, M.valA);
    } else {
      m.stat = SADR;
    }
  }
  M2W();
}
void Device::M2W() {
  W.stat = m.stat;
  W.icode = M.icode;
  W.valE = M.valE;
  W.valM = m.valM;
  W.dstE = M.dstE;
  W.dstM = M.dstM;
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
bool Device::IfRet() const {
  return In(E.icode, IMRMOVQ, IPOPQ) && In(E.dstM, d.srcA, d.srcB);
}
uint8_t Device::GetFControl() const {
  if (IfLoadUseH() || IfRet()) {
    return CSTALL;
  } else {
    return CNORMAL;
  }
}
uint8_t Device::GetDControl() const {
  if (IfLoadUseH()) {
    return CSTALL;
  } else if (IfRet() || IfMispredicted()) {
    return CBUBBLE;
  } else {
    return CNORMAL;
  }
}
uint8_t Device::GetEControl() const {
  if (IfMispredicted() || IfLoadUseH()) {
    return CBUBBLE;
  } else {
    return CNORMAL;
  }
}
