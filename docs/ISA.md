# Y86-64 Extented ISA文档

## 寄存器设计

共设置15个通用寄存器，按如下顺序表示

```
        '%rax': 0,
        '%rcx': 1,
        '%rdx': 2,
        '%rbx': 3,
        '%rsp': 4,
        '%rbp': 5,
        '%rsi': 6,
        '%rdi': 7,
        "%r8": 8,
        "%r9": 9,
        "%r10": 10,
        "%r11": 11,
        "%r12": 12,
        "%r13": 13,
        "%r14": 14
```

## 指令集设计

### halt

- icode：0
- 长度：1
- 动作：CPU停机

### nop

- icode: 1
- 长度：1
- 动作：无

### rrmovq

- icode: 2
- 长度: 2
- 操作数：%ra, %rb
- 动作：将%ra的值移动至%rb中

#### rrmovq

- ifun：0
- 动作：无条件移动

#### Cmovle

- ifun: 1
- 动作：小于等于时移动

#### cmovl

- ifun: 2
- 动作：小于时移动

#### cmove

- Ifun：3
- 动作：等于时移动

#### cmovne

- Ifun: 4
- 动作：不等于时移动

#### cmovge

- Ifun：5
- 动作：大于等于时移动

#### cmovg

- Ifun: 6
- 动作：大于时移动

### irmovq

- icode：3
- 长度：10
- 操作数：$imm %rb
- 动作：将$imm的值移动至%rb中

### rmmovq

- Icode：4
- 长度：10
- 操作数: %rb %ra $imm
- 动作：将%ra的值写入%rb+imm的地址处，如果%rb为RNONE，则直接写入imm的地址处，如果imm没有写，则视为0

#### rmmovq

- Ifun: 0
- 动作：移动8个byte

#### rmmovl

- Ifun：1
- 动作：移动低4个byte

#### rmmovw

- ifun: 2
- 动作：移动低2个byte

#### rmmovb

- Ifun: 3
- 动作：移动低1个byte

### mrmov

- icode: 5
- 长度:10
- 操作数：%ra %rb $ imm
- 动作：将%rb+imm的地址处的值写入%ra，如果%rb为RNONE，则直接写入imm的地址处，如果imm没有写，则视为0

#### mrmovq

- Ifun：0
- 动作：移动8byte

#### mrmovlu

- ifun: 1
- 动作：移动至低4byte，高4位用0填补

#### mrmovwu

- Ifun: 2
- 动作：移动至低2byte，高位用0填补

#### mrmovbu

- ifun: 3
- 动作：移动至低1byte，高位用0填补

#### mrmovls

- Ifun: 4
- 动作：移动至低4byte，高位用符号位填补

#### mrmovws

- ifun: 5
- 动作：移动至低2byte，高位用符号位填补

#### mrmovbs

- ifun: 6
- 动作：移动至低1byte，高位用符号位填补

#### OPq

- icode: 5
- 长度：2 
- 操作数：%ra, %rb
- 动作：执行%rb alu %ra,将结果写入%rb中

```
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
```



