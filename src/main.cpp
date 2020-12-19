//
// Created by 谢子飏 on 2020/12/02.
//

#include <ostream>

#include "Controller.h"
#include "Output.h"
int main() {
    Controller c;
    /* 下面的raw code的汇编源码
     *  0x000:                         |     .pos 0
        0x000: 30f00000000000000000    |     irmovq $0 %rax
        0x00a: 30f16400000000000000    |     irmovq $100 %rcx
        0x014: 30f20100000000000000    |     irmovq $1 %rdx
        0x01e:                         | loop:
        0x01e: 6010                    |     addq %rcx %rax
        0x020: 6121                    |     subq %rdx %rcx
        0x022: 733400000000000000      |     je end
        0x02b: 701e00000000000000      |     jmp loop
        0x034:                         | end:
        0x034: 00                      |     halt

     */
    std::string raw_code(
        "4b0000000000000030f74b0000000000000030f6000900000000000030f10800000000"
        "00000050070000000000000000400f0009000000000000601760165007000000000000"
        "0000400600000000000000000068656c6c6f2c776f726c6421");
    c.FlashCode(raw_code);
    c.Run(true);
    // std::cout << c.GetConsoleOutput();
    // OutputFinal(c.d);
}