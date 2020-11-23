from devices import Devices
class instr:
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
        self.icode,self.ifun = self.read_split_byte(self.storage.PC)
        self.ra,self.rb = self.read_split_byte(self.storage.PC+1)

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

    def write_8_bytes(self, start_pos, val)->None:
        while val > 0:
            self.storage.Mem[start_pos] = val&0xff
            val = val >> 8

    def read_8_bytes(self, start_pos)->int:
        res = 0
        for i in range(8):
            res += self.storage.Mem[start_pos+i] << (8*i)
        return res

    def read_split_byte(self, pos)->(int, int):
        return self.storage.Mem[pos]&0xf, self.storage.Mem[pos]>>4

    def CC(self):
        return (True,
                self.storage.ZF == 1 or (self.storage.SF ^ self.storage.OF) == 1,
                (self.storage.SF ^ self.storage.OF) == 1,
                self.storage.ZF == 1,
                self.storage.ZF == 0,
                (self.storage.SF ^ self.storage.OF) == 0,
                self.storage.ZF == 0 or (self.storage.SF ^ self.storage.OF) == 0
                )


class OPq(instr):

    def Fetch(self):
        super(OPq, self).Fetch()
        self.valP = self.storage.PC + 2

    def Decode(self):
        self.valA = self.storage.Reg[self.ra]
        self.valB = self.storage.Reg[self.rb]

    def Execute(self):
        choice = (lambda x, y: x + y, lambda x, y: x - y, lambda x, y: x & y, lambda x, y: x ^ y)
        self.valE = choice[self.ifun](self.valB, self.valA)
        if self.valE == 0:
            self.storage.ZF = 1
        elif self.valE < 0:
            self.storage.SF = 1
        elif self.valE >= (1 << 64) or self.valE <= -(1 << 64):
            self.storage.OF = 1

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
        self.storage.Reg[self.rb] = self.valE


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
        self.valP = self.storage.PC + 20

    def Decode(self):
        self.valB = self.storage.Reg[self.rb]

    def Execute(self):
        self.valE = self.valB + self.valC

    def Memory(self):
        self.valM = self.read_8_bytes(self.storage.PC + 2)

    def Write_back(self):
        self.storage.Reg[self.ra] = self.valM


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
        self.icode,self.ifun = self.read_split_byte(self.storage.PC)
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
        self.icode,self.ifun = self.read_split_byte(self.storage.PC)
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
        self.icode,self.ifun = self.read_split_byte(self.storage.PC)
        self.valP = self.storage.PC

    def Decode(self):
        self.valA = self.storage.Reg[4]
        self.valA = self.storage.Reg[4]

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