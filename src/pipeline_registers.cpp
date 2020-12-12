//
// Created by 王少文 on 2020/12/1.
//
#include "pipeline_registers.h"
#include "Device.h"
#include "instr.h"
void W_Reg::reset() {
    stat = SBUB;
    icode = INOP;
    dstE = RNONE;
  dstM = RNONE;
}
void M_Reg::reset() {
    stat = SBUB;
    icode = INOP;
    dstE = RNONE;
  dstM = RNONE;
}
void E_Reg::reset() {
    stat = SBUB;
    icode = INOP;
    dstE = RNONE;
  dstM = RNONE;
  srcA = RNONE;
  srcB = RNONE;
}
void D_Reg::reset() {
    stat = SBUB;
    icode = INOP;
    rA = RNONE;
  rB = RNONE;
}
void F_Reg::reset() {}
