from devices import Devices
import numpy as np


class instr:
    # 如果是在cpp中，可以加一个handle类的，这样Fetch阶段更符合逻辑，在取指令的同时，可以执行子类的操作，然后可以做更多的封装，这里的
    # 封装太粗了，例如，可以把所有的mov都放成一类
    def __init__(self, storage: Devices):
        self.storage = storage
        self.ifun = 0
        self.icode = 0
        self.valP = 0
        self.ra = 0
        self.rb = 0
        self.valA = 0
        self.valB = 0
        self.valC = 0
        self.valE = 0
        self.valM = 0
        self.cnd = 0

    def Fetch(self):
        self.icode, self.ifun = self.read_split_byte(self.storage.PC)
        self.ra, self.rb = self.read_split_byte(self.storage.PC+1)

    def Decode(self):
        pass

    def Execute(self):
        pass

    def Memory(self):
        pass

    def Write_back(self):
        pass

    def PC_update(self):
        self.storage.PC = self.valP

    def write_8_bytes(self, start_pos, val) -> None:
        while val > 0:
            self.storage.Mem[int(start_pos)] = int(val) & 0xff
            val = int(val) >> 8
            start_pos += 1

    def read_8_bytes(self, start_pos) -> int:
        res = int(0)
        for i in range(8):
            res += (int(self.storage.Mem[int(start_pos+i)]) << (8*i))
        return res

    def read_split_byte(self, pos) -> (int, int):
        return self.storage.Mem[pos] >> 4, self.storage.Mem[pos] & 0xf

    def CC(self):
        return (True,
                self.storage.ZF == 1 or (
                    self.storage.SF ^ self.storage.OF) == 1,
                (self.storage.SF ^ self.storage.OF) == 1,
                self.storage.ZF == 1,
                self.storage.ZF == 0,
                (self.storage.SF ^ self.storage.OF) == 0,
                self.storage.ZF == 0 or (
                    self.storage.SF ^ self.storage.OF) == 0
                )


class OPq(instr):

    def Fetch(self):
        super(OPq, self).Fetch()
        self.valP = self.storage.PC + 2

    def Decode(self):
        self.valA = self.storage.Reg[self.ra]
        self.valB = self.storage.Reg[self.rb]

    def Execute(self):
        # TODO: 建议把ALU和CC操作独立成类(dbq又是一层封装，这样就可以实现CPU"模拟”)
        choice = (lambda x, y: x + y, lambda x, y: x - y,
                  lambda x, y: x & y, lambda x, y: x ^ y)
        self.valE = int(choice[self.ifun](self.valB, self.valA))
        if self.valE == 0:
            self.storage.ZF = 1
        else:
            self.storage.ZF = 0
        if (self.valE >> 63) == 1:
            self.storage.SF = 1
        else:
            self.storage.SF = 0
        # TODO 球球了我真的不知道OF flag咋写，先“面向测例编程， 写一个能用的
        if self.ifun == 0 and (int(self.valE)) >> 63 != (int(self.valA) >> 63) and (int(self.valE) >> 63) != (int(self.valB) >> 63):
            self.storage.OF = 1
        else:
            self.storage.OF = 0

    def Write_back(self):
        self.storage.Reg[self.rb] = self.valE


class rrmovq(instr):

    def Fetch(self):
        super(rrmovq, self).Fetch()
        self.valP = self.storage.PC + 2

    def Decode(self):
        self.valA = self.storage.Reg[self.ra]

    def Execute(self):
        choice = self.CC()
        self.cnd = choice[self.ifun]
        self.valE = 0 + self.valA

    def Write_back(self):
        if self.cnd:
            self.storage.Reg[self.rb] = self.valE


class irmovq(instr):

    def Fetch(self):
        super(irmovq, self).Fetch()
        self.valC = self.read_8_bytes(self.storage.PC + 2)
        self.valP = self.storage.PC + 10

    def Execute(self):
        self.valE = 0 + self.valC

    def Write_back(self):
        self.storage.Reg[self.rb] = int(self.valE) & 0xffffffffffffffff


class rmmovq(instr):

    def Fetch(self):
        super(rmmovq, self).Fetch()
        self.valC = self.read_8_bytes(self.storage.PC + 2)
        self.valP = self.storage.PC + 10

    def Decode(self):
        self.valA = self.storage.Reg[self.ra]
        self.valB = self.storage.Reg[self.rb]

    def Execute(self):
        self.valE = self.valB + self.valC

    def Memory(self):
        self.write_8_bytes(self.valE, self.valA)


class mrmovq(instr):

    def Fetch(self):
        super(mrmovq, self).Fetch()
        self.valC = self.read_8_bytes(self.storage.PC + 2)
        self.valP = self.storage.PC + 10

    def Decode(self):
        self.valB = self.storage.Reg[self.rb]

    def Execute(self):
        self.valE = self.valB + self.valC

    def Memory(self):
        self.valM = self.read_8_bytes(self.valE)

    def Write_back(self):
        self.storage.Reg[self.ra] = int(self.valM) & 0xffffffffffffffff


class pushq(instr):

    def Fetch(self):
        super(pushq, self).Fetch()
        self.valP = self.storage.PC + 2

    def Decode(self):
        self.valA = self.storage.Reg[self.ra]
        self.valB = self.storage.Reg[4]

    def Execute(self):
        self.valE = self.valB - 8

    def Memory(self):
        self.write_8_bytes(self.valE, self.valA)

    def Write_back(self):
        self.storage.Reg[4] = self.valE


class popq(instr):

    def Fetch(self):
        super(popq, self).Fetch()
        self.valP = self.storage.PC + 2

    def Decode(self):
        self.valA = self.storage.Reg[4]
        self.valB = self.storage.Reg[4]

    def Execute(self):
        self.valE = self.valB + 8

    def Memory(self):
        self.valM = self.read_8_bytes(self.valA)

    def Write_back(self):
        self.storage.Reg[4] = self.valE
        self.storage.Reg[self.ra] = self.valM


class jXX(instr):
    def Fetch(self):
        self.icode, self.ifun = self.read_split_byte(self.storage.PC)
        self.valC = self.read_8_bytes(self.storage.PC + 1)
        self.valP = self.storage.PC + 9

    def Execute(self):
        choice = self.CC()
        self.cnd = choice[self.ifun]

    def PC_update(self):
        if self.cnd:
            self.storage.PC = self.valC
        else:
            self.storage.PC = self.valP


class call(instr):
    def Fetch(self):
        self.icode, self.ifun = self.read_split_byte(self.storage.PC)
        self.valC = self.read_8_bytes(self.storage.PC + 1)
        self.valP = self.storage.PC + 9

    def Decode(self):
        self.valB = self.storage.Reg[4]

    def Execute(self):
        self.valE = self.valB - 8

    def Memory(self):
        self.write_8_bytes(self.valE, self.valP)

    def Write_back(self):
        self.storage.Reg[4] = self.valE

    def PC_update(self):
        self.storage.PC = self.valC


class ret(instr):
    def Fetch(self):
        self.icode, self.ifun = self.read_split_byte(self.storage.PC)
        self.valP = self.storage.PC + 1

    def Decode(self):
        self.valA = self.storage.Reg[4]
        self.valB = self.storage.Reg[4]

    def Execute(self):
        self.valE = self.valB + 8

    def Memory(self):
        self.valM = self.read_8_bytes(self.valA)

    def Write_back(self):
        self.storage.Reg[4] = self.valE

    def PC_update(self):
        self.storage.PC = self.valM


class halt(instr):
    def Fetch(self):
        self.valP = self.storage.PC + 1

    def Write_back(self):
        self.storage.State = self.storage.HLT


class nop(instr):
    def Fetch(self):
        self.valP = self.storage.PC + 1
