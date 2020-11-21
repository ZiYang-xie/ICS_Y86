#ifndef DECODER
#define DECODER
#pragma once

#include <cstdint>
#include <string>

#include "../cpu/kernels/register.h"

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
    // 构造和析构
    StdInstr() : op_id_(0), reg_flag_(false), imm_flag_(false), imm_(0){};
    StdInstr(const std::string&);
    ~StdInstr();

    // getter & setter
    const OperationSet getOp_();
    const uint_fast8_t getopId_();
    const Register getRegId_();
    const int_fast64_t getImm_();
    const bool getRegFlag_();
    const bool getImmFlag_();

    const OperationSet setOp_();
    const uint_fast8_t setopId_();
    const Register setRegId_();
    const int_fast64_t setImm_();
    const bool setRegFlag_();
    const bool setImmFlag_();

   private:
    OperationSet op_;
    uint_fast8_t op_id_;

    bool reg_flag_;
    Register reg_id_;

    bool imm_flag_;
    int_fast64_t imm_;
};

#endif