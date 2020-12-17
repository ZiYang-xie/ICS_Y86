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
        "600000000000000030f0000000000000000030f1640000000000000030f2010000000000000060106121733400000000000000701e0000000000000030f0000000000000000030f1640000000000000030f201000000000000006010612174520000000000000000");
    c.FlashCode(raw_code);
    c.Run(true);
    // std::cout << c.GetConsoleOutput();
    // OutputFinal(c.d);
}