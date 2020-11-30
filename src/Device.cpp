//
// Created by 王少文 on 2020/11/27.
//
#include "Device.h"
#include "instr.h"
#include "util.h"
#include <cstring>
#include <stdexcept>

Device::Device(const char *str)
    : Reg{}, Mem{}, CFLAG{}, Stat{SAOK}, F{}, D{}, E{}, M{}, W{}, f{}, d{}, e{},
      m{} {
  memset(Reg, 0, REG_SIZE);
  memset(CFLAG, 0, 3);
  memset(Mem, 0, MEM_SIZE);
  if (strlen(str) >= MEM_SIZE - RESERVED_SIZE) {
    throw std::domain_error("String is too long to flash");
  }
  int i = 0;
  while (str) {
    Mem[i++] = CharToUint8(*str++);
  }
}
bool Device::IfAddrValid(uint64_t pos) {
  return pos >= 0 && pos <= MEM_SIZE - RESERVED_SIZE;
}
bool Device::IfInstrValid(uint8_t icode) { return icode >= 0 && icode <= 0xe; }
uint8_t Device::ReadHigh4Bits(uint64_t pos) const { return Mem[pos] & 0xf0u; }
uint8_t Device::ReadLow4Bits(uint64_t pos) const { return Mem[pos] & 0x0fu; }
uint64_t Device::Read8Bytes(uint64_t pos) const {
  return *(uint64_t *)(Mem + pos);
}
void Device::Write8Bytes(uint64_t pos, uint64_t val) {
  *(uint64_t *)(Mem + pos) = val;
}
void Device::Fetch() {
  uint8_t f_pc;
  if (M.icode == IJXX && !M.Cnd) {
    f_pc = M.valA; // JXX但是没有成功跳转的情况
  } else if (M.icode == IRET) {
    f_pc = W.valM; // Ret的情况，后期使用硬件栈时可以删去这一句话
  } else {
    f_pc = F.predPC;
  }
  f.icode = ReadLow4Bits(f_pc);
  f.ifun = ReadHigh4Bits(f_pc);
  if (IfInstrValid(f.icode)) {
    f.Stat = SINS;
  } else if (f.icode == IHALT) {
    f.Stat = SHLT;
  }
  bool need_regids =
      In(f.icode, IIRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ);
  bool need_valC = In(f.icode, IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL);
  if (need_regids) {
    f.ra = ReadLow4Bits(f_pc + 1);
    f.rb = ReadHigh4Bits(f_pc + 1);
  }
  if (need_valC) {
    f.valC = ReadHigh4Bits(f_pc + need_regids + 1);
  }
  f.valP = f_pc + 1 + need_regids + 8 * need_valC;
  SetFPredPc();
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
  F2D();
  //写srcA
  if (In(D.icode, IIRMOVQ, IRMMOVQ, IOPQ, IPUSHQ)) {
    d.srcA = D.rA;
  } else if (In(D.icode, IPOPQ, IRET)) {
    d.srcA = RRSP;
  } else {
    d.srcA = RNONE;
  }
  //写srcB
  if (In(D.icode, IOPQ, IRMMOVQ)) {
    d.srcB = D.rA;
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
}
void Device::Execute() {
  D2E();
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
std::function<int(int, int)> Device::GetALUFunc(uint8_t ifun) {
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
