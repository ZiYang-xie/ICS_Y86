//
// Created by 谢子飏 on 2020/12/02.
//

#include "Controller.h"
#include "Output.h"

int main() {
    Controller c;
    /* 下面的raw code的汇编源码
     *  0x000:                         |     .pos 0
        0x000: 30f74b00000000000000    |     irmovq str %rdi
        0x00a: 30f60009000000000000    |     irmovq $0x900 %rsi
        0x014: 30f10800000000000000    |     irmovq $8 %rcx
        0x01e: 50070000000000000000    |     mrmovq (%rdi) %rax
        0x028: 40060000000000000000    |     rmmovq %rax (%rsi)
        0x032: 6017                    |     addq %rcx %rdi
        0x034: 6016                    |     addq %rcx %rsi
        0x036: 50070000000000000000    |     mrmovq (%rdi) %rax
        0x040: 40060000000000000000    |     rmmovq %rax (%rsi)
        0x04a: 00                      |     halt
        0x04b:                         | str:
        0x04b: 68656c6c6f2c776f726c6421|     .string hello,world!
     */
    std::string raw_code(
        "30f74b0000000000000030f6000900000000000030f1080000000000000050070000000000000000400600000000000000006017601650070000000000000000400600000000000000000068656c6c6f2c776f726c6421");
    c.FlashCode(raw_code);
    c.Run(10000, false);
    std::cout << c.GetConsoleOutput();
    // OutputFinal(c.d);
}