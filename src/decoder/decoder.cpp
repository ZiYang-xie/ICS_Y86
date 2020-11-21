#include "decoder.h"

StdInstr::StdInstr(const std::string& raw) {
    switch (raw[0]) {
        case halt:
            op_ = halt;
            break;
        case nop:
            op_ = nop;
            break;
        case Cmov:
            op_ = Cmov;
            // TODO: Handle Cmov
            break;
        case irmovq:
            op_ = irmovq;
            break;
        case rmmovq:
            op_ = rmmovq;
            break;
        case mrmovq:
            op_ = mrmovq;
            break;
        case OPq:
            op_ = OPq;
            // TODO: Handle OPq
            break;
        case Jmp:
            op_ = Jmp;
            // TODO: Handle Jmp
            break;
        case call:
            op_ = call;
            break;
        case ret:
            op_ = ret;
            break;
        case pushq:
            op_ = pushq;
            break;
        case popq:
            op_ = popq;
            break;
        default:
            // TODO: Error
            break;
    }
}


// TODO: A buntch of getter & setter func