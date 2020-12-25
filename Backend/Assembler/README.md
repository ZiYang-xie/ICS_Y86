# 该如何使用yyas

## 文件架构

- yyas: yyas汇编器
- test.py: yyas汇编器的测试代码，若需要运行test.py，需要在同一文件夹下，放置官方给出的test_ys和test_yo文件，来验证我们的汇编器在基础的ys文件上，与官方的汇编器能有相同的效果。使用python3 test.py

## yyas的使用方法

```sh
usage: yyas [-h] [-o OUTFILE] [-r] [-np] [-v] sourcefile

yyas: Assemble .ys file to .yo file or/and .yo file to raw byte file

positional arguments:
  sourcefile            The source file to be assembled

optional arguments:
  -h, --help            show this help message and exit
  -o OUTFILE, --output OUTFILE
                        Assign the output file
  -r, --raw             Generate raw byte file
  -np, --noprefix       Do not generate prefix in raw output
  -v, --version         show version
```

## 如何修改yyas来适应你的ISA

1. yyas设计时假定在类unix系统上运行(Mac OSX和Linux)，如果你在windows上运行，请加上.py后缀，并删掉代码首行的`#! /usr/bin/python3`

2. 你需要给你的新增的指令创造一个类，继承自Instr，然后修改instr和gen_byte_code函数

## 我的碎碎念

目前代码中还有许多不规范的地方，也可能有bug。此外目前的错误提示还比较简陋。如果你在运行时发现了bug，欢迎使用issue功能或发起pull request
