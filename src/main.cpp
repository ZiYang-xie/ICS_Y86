//
// Created by 谢子飏 on 2020/12/02.
//

#include "Controller.h"
#include "Output.h"

int main() {
    Controller c;
    /* 下面的raw code的汇编源码
     *  0x000:                         |     .pos 0
        0x000: 30f74b00000000000000    |     irmovq string %rdi
        0x00a: 30f60009000000000000    |     irmovq $0x900 %rsi
        0x014: 30f10800000000000000    |     irmovq $8 %rcx
        0x01e: 50070000000000000000    |     mrmovq (%rdi) %rax
        0x028: 40060000000000000000    |     rmmovq %rax (%rsi)
        0x032: 6017                    |     addq %rcx %rdi
        0x034: 6016                    |     addq %rcx %rsi
        0x036: 50070000000000000000    |     mrmovq (%rdi) %rax
        0x040: 40060000000000000000    |     rmmovq %rax (%rsi)
        0x04a: 00                      |     halt
        0x04b:                         | string:
        0x04b: 48                      |     .byte 0x48
        0x04c: 65                      |     .byte 0x65
        0x04d: 6c                      |     .byte 0x6c
        0x04e: 6c                      |     .byte 0x6c
        0x04f: 6f                      |     .byte 0x6f
        0x050: 2c                      |     .byte 0x2c
        0x051: 77                      |     .byte 0x77
        0x052: 6f                      |     .byte 0x6f
        0x053: 72                      |     .byte 0x72
        0x054: 6c                      |     .byte 0x6c
        0x055: 64                      |     .byte 0x64
        0x056: 21                      |     .byte 0x21
     */
    std::string raw_code(
        "30f74b0000000000000030f6000900000000000030f108000000000000005007000000"
        "0000000000400600000000000000006017601650070000000000000000400600000000"
        "000000000048656c6c6f2c776f726c6421");
    c.FlashCode(raw_code);
    c.Run(10000, false);
    std::cout << c.GetConsoleOutput();
    // OutputFinal(c.d);
}