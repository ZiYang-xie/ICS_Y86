//
// Created by 王少文 on 2020/11/27.
//
#include <cstdint>
#ifndef ICS_Y86_PIPELINE_REGISTERS_H
#define ICS_Y86_PIPELINE_REGISTERS_H
const uint8_t RRAX = 0;
const uint8_t RRCX = 1;
const uint8_t RRDX = 2;
const uint8_t RRBX = 3;
const uint8_t RRSP = 4;
const uint8_t RRBP = 5;
const uint8_t RRSI = 6;
const uint8_t RRDI = 7;
const uint8_t RR8 = 8;
const uint8_t RR9 = 9;
const uint8_t RR10 = 10;
const uint8_t RR11 = 11;
const uint8_t RR12 = 12;
const uint8_t RR13 = 13;
const uint8_t RR14 = 14;
const uint8_t RNONE = 15;
const uint8_t CNORMAL = 0;
const uint8_t CBUBBLE = 1;
const uint8_t CSTALL = 2;
struct W_Reg {
  uint8_t stat{};
  uint8_t icode{};
  uint64_t valE{};
  uint64_t valM{};
  uint8_t dstE{RNONE};
  uint8_t dstM{RNONE};
  void reset();
};
struct M_Reg {
  uint8_t stat{};
  uint8_t icode{};
  bool Cnd{};
  uint64_t valE{};
  uint64_t valA{};
  uint8_t dstE{RNONE};
  uint8_t dstM{RNONE};
  void reset();
};
struct E_Reg {
  uint8_t stat{};
  uint8_t icode{};
  uint8_t ifun{};
  uint64_t valC{};
  uint64_t valA{};
  uint64_t valB{};
  uint8_t dstE{RNONE};
  uint8_t dstM{RNONE};
  uint8_t srcA{RNONE};
  uint8_t srcB{RNONE};
  void reset();
};
struct D_Reg {
  uint8_t stat{};
  uint8_t icode{};
  uint8_t ifun{};
  uint8_t rA{RNONE};;
  uint8_t rB{RNONE};;
  uint64_t valC{};
  uint64_t valP{};
  void reset();
};
struct F_Reg {
  uint64_t predPC;
  void reset();
};
struct f_wire {
  uint8_t icode;
  uint8_t ifun;
  uint8_t Stat;
  uint8_t ra;
  uint8_t rb;
  uint64_t valC;
  uint64_t valP;
};
struct d_wire {
  uint64_t SelFwdA;
  uint64_t FwdB;
  uint8_t dstE;
  uint8_t dstM;
  uint8_t srcA;
  uint8_t srcB;
  uint64_t valA;
  uint64_t valB;
};
struct e_wire {
  bool Cnd;
  uint8_t dstE;
  uint64_t valE;
};
struct m_wire {
  uint8_t stat;
  uint64_t valM;
};

#endif // ICS_Y86_PIPELINE_REGISTERS_H
