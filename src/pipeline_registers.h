//
// Created by 王少文 on 2020/11/27.
//

#ifndef ICS_Y86_PIPELINE_REGISTERS_H
#define ICS_Y86_PIPELINE_REGISTERS_H
#include <cstdint>

#include "index_const.h"
#include "instr.h"
struct W_Reg {
  uint8_t stat{SAOK};
  uint8_t icode{};
  uint64_t valE{};
  uint64_t valM{};
  uint8_t dstE{RNONE};
  uint8_t dstM{RNONE};
  void reset();
};
struct M_Reg {

  uint8_t stat{SAOK};
  uint8_t icode{};
  bool Cnd{};
  uint64_t valE{};
  uint64_t valA{};
  uint8_t dstE{RNONE};
  uint8_t dstM{RNONE};
  void reset();
};
struct E_Reg {
    uint8_t control {CNORMAL};
  uint8_t stat{SAOK};
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
    uint8_t control {CNORMAL};
  uint8_t stat{SAOK};
  uint8_t icode{};
  uint8_t ifun{};
  uint8_t rA{RNONE};;
  uint8_t rB{RNONE};;
  uint64_t valC{};
  uint64_t valP{};
  void reset();
};
struct F_Reg {
    uint8_t control{CNORMAL};
  uint64_t predPC{0};
  void reset();
};
struct f_wire {
  uint8_t icode{INOP};
  uint8_t ifun{};
  uint8_t Stat{SAOK}   ;
  uint8_t ra{RNONE};
  uint8_t rb{RNONE};
  uint64_t valC{};
  uint64_t valP{};
};
struct d_wire {
    uint8_t stat{SAOK};
    uint8_t icode{INOP};
    uint8_t ifun{};
    uint64_t valC{};
  uint64_t SelFwdA{};
  uint64_t FwdB{};
  uint8_t dstE{RNONE};
  uint8_t dstM{RNONE};
  uint8_t srcA{RNONE};
  uint8_t srcB{RNONE};
  uint64_t valA{};
  uint64_t valB{};
};
struct e_wire {
    bool Cnd{};
    bool CFLAG[3]{};
    uint8_t stat{SAOK};
    uint8_t icode{INOP};
  uint8_t dstE{RNONE};
  uint64_t valE{};
  uint64_t valA{};
  uint64_t dstM{};
};
struct m_wire {
    uint8_t icode{INOP};
    uint64_t valE{};
    uint64_t dstE{};
    uint64_t dstM{};
  uint8_t stat{SAOK};
  uint64_t valM{};
};

#endif // ICS_Y86_PIPELINE_REGISTERS_H
