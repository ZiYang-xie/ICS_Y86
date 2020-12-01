//
// Created by 王少文 on 2020/11/28.
//
#ifndef ICS_Y86_INSTR_H
#define ICS_Y86_INSTR_H
#include <cstdint>


const uint8_t IHALT = 0;
const uint8_t INOP = 1;
const uint8_t IRRMOVQ = 2;
const uint8_t IIRMOVQ = 3;
const uint8_t IRMMOVQ = 4;
const uint8_t IMRMOVQ = 5;
const uint8_t IOPQ = 6;
const uint8_t IJXX = 7;
const uint8_t ICALL = 8;
const uint8_t IRET = 9;
const uint8_t IPUSHQ = 0xa;
const uint8_t IPOPQ = 0xb;
const uint8_t ALUADD = 0;
const uint8_t ALUSUB = 1;
const uint8_t ALUAND = 2;
const uint8_t ALUXOR = 3;
// Branch
const uint8_t BALWAYS = 0;
const uint8_t BLE = 1;
const uint8_t BL = 2;
const uint8_t BE = 3;
const uint8_t BNE = 4;
const uint8_t BGE = 5;
const uint8_t BG = 6;



#endif // ICS_Y86_INSTR_H