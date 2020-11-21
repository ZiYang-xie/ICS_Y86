#ifndef STDINSTR
#define STDINSTR
#pragma once

#include "../cpu/kernels/register.h"
#include <cstdint>

enum OperationSet {
    halt = 0,
    nop = 1,
    Cmov = 2,
    irmovq = 3,
    rmmovq = 4,
    mrmovq = 5,
    OPq = 6,
    Jmp = 7,
    call = 8,
    ret = 9,
    pushq = 10,
    popq = 11
};

class StdInstr {
   public:
    
   private:
    OperationSet op_;
    uint_fast8_t id_;

    bool reg_flag_;
    Register reg_id_;

    bool imm_flag_;
    int_fast64_t imm_;
};

#endif