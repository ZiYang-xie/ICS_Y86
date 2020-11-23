import numpy as np


class Devices:
    AOK = 1
    HLT = 2
    ADR = 3
    INS = 4

    def __init__(self, size=200):
        self.ZF = 0
        self.SF = 0
        self.OF = 0
        self.PC = 0
        self.Reg = np.zeros(15, dtype=np.uint64)
        self.Mem = np.zeros(size, dtype=np.uint8)
        self.State = Devices.AOK

    def __str__(self):
        res = "-----------------------\n"
        res += "PC: " + hex(
            self.PC) + " \t" + " ".join(list(map(lambda a: hex(a), self.Mem[self.PC: self.PC + 10]))) + '\n'
        res += "ZF: " + hex(self.ZF) + "\tSF: " + \
               hex(self.SF) + "\tOF: " + hex(self.OF) + '\n'
        reg_list = (
            "%rax", "%rcx", "%rdx", "%rbx",
            "%rsp", "%rbp", "%rsi", "%rdi",
            "%r8", "%r9", "%r10", "%r11",
            "%r12", "%r13", "%r14")
        for i in range(5):
            res += reg_list[i] + ': ' + hex(self.Reg[i]) + '\t'
        res += '\n'
        for i in range(5, 10):
            res += reg_list[i] + ': ' + hex(self.Reg[i]) + '\t'
        res += '\n'
        for i in range(10, 15):
            res += reg_list[i] + ': ' + hex(self.Reg[i]) + '\t'
        res += '\n'
        return res

    def str_memcpy(self, start_pos, s: str):
        if len(s) + start_pos > len(self.Mem):
            raise ValueError("string is too long ")
        for i in range(0, len(s), 2):
            self.Mem[start_pos] = int(s[i + 1], 16) + (int(s[i], 16) << 4)
            start_pos += 1
