# 指令集扩展

## rmmovq/mrmovq/irmovq

Mem支持仅常量(将reg位置为0xf)
支持movb,movw,movl,movwu,movlu

## OPq

ifun =

0. addq
1. subq
2. andq
3. xorq
4. orq
5. shl (unsigned shift left)/sal，共享一个机器码
6. shr (unsigned shift right)
7. sar (signed shift right)
8. mulq (multiply, only low 64-bit)
9. divq (divide)
10. remq (reminder)
11. minq (Minimum, from RISC-V)
12. maxq (Maximum, from RISC-V)

## iOPq

icode = 0xb iOPq imm reg，其余与OPq相同

## testq

icode = 0xc

## cmpq

icode = 0xd

## icmpq

icode = 0xe

Usage: icmpq $imm %reg
