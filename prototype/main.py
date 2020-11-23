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
        self.Reg = [0 for _ in range(15)]
        self.Mem = "0" * size
        self.State = Devices.AOK

    def __str__(self):
        res = "-----------------------\n"
        res += "PC: " + str(self.PC) + '\n'
        res += "ZF: " + str(self.ZF) + "\tSF: " + str(self.SF) + "\tOF: " + str(self.OF) + '\n'
        reg_list = (
            "%rax", "%rcx", "%rdx", "%rbx",
            "%rsp", "%rbp", "%rsi", "%rdi",
            "%r8", "%r9", "%r10", "%r11",
            "%r12", "%r13", "%r14")
        for i in range(4):
            res += reg_list[i] + ': ' + str(self.Reg[i]) + '\t'
        res += '\n'
        for i in range(4, 8):
            res += reg_list[i] + ': ' + str(self.Reg[i]) + '\t'
        res += '\n'
        for i in range(8, 12):
            res += reg_list[i] + ': ' + str(self.Reg[i]) + '\t'
        res += '\n'
        for i in range(12, 15):
            res += reg_list[i] + ': ' + str(self.Reg[i]) + '\t'
        res += '\n'
        return res

    def insert_code(self, s):
        self.Mem = s + self.Mem


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
        self.ifun = self.read_half_byte(self.storage.PC + 0)
        self.icode = self.read_half_byte(self.storage.PC + 1)
        self.ra = self.read_half_byte(self.storage.PC + 4)
        self.rb = self.read_half_byte(self.storage.PC + 3)

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

    @staticmethod
    def reverse_str(s):
        if len(s) % 2 != 0:
            raise ValueError("s must have even length")
        if s == "":
            return ""
        else:
            return instr.reverse_str(s[2:]) + s[0:2]

    @staticmethod
    def str2int(s):
        return int(instr.reverse_str(s), 16)

    @staticmethod
    def int2str(n):
        s = hex(n)[2:]
        while len(s) < 16:
            s = '0' + s
        return instr.reverse_str(s)

    def write_8_bytes(self, start_pos, val):
        self.storage.Mem = self.storage.Mem[:start_pos] + instr.int2str(val) + self.storage.Mem[start_pos + 16:]

    def read_8_bytes(self, start_pos):
        return instr.str2int(self.storage.Mem[start_pos:start_pos + 16])

    def read_half_byte(self, pos):
        return int(self.storage.Mem[pos], 16)

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
        self.valP = self.storage.PC + 4

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
        self.valP = self.storage.PC + 4

    def Decode(self):
        self.valA = self.storage.Reg[self.ra]

    def Execute(self):
        choice = self.CC()
        self.cnd = choice[self.ifun]
        self.valE = 0 + self.valE

    def Write_back(self):
        if self.cnd:
            self.storage.Reg[self.rb] = self.valE


class irmovq(instr):

    def Fetch(self):
        super(irmovq, self).Fetch()
        self.valC = self.read_8_bytes(self.storage.PC + 4)
        self.valP = self.storage.PC + 20

    def Execute(self):
        self.valE = 0 + self.valC

    def Write_back(self):
        self.storage.Reg[self.rb] = self.valE


class rmmovq(instr):

    def Fetch(self):
        super(rmmovq, self).Fetch()
        self.valC = self.read_8_bytes(self.storage.PC + 4)
        self.valP = self.storage.PC + 20

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
        self.valC = instr.str2int(self.storage.Mem[self.storage.PC + 4:self.storage.PC + 20])
        self.valP = self.storage.PC + 20

    def Decode(self):
        self.valB = self.storage.Reg[self.rb]

    def Execute(self):
        self.valE = self.valB + self.valC

    def Memory(self):
        self.valM = self.read_8_bytes(self.storage.PC + 4)

    def Write_back(self):
        self.storage.Reg[self.ra] = self.valM


class pushq(instr):

    def Fetch(self):
        super(pushq, self).Fetch()
        self.valP = self.storage.PC + 4

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
        self.valP = self.storage.PC + 4

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
        self.icode = self.read_half_byte(self.storage.PC + 0)
        self.ifun = self.read_half_byte(self.storage.PC + 1)
        self.valC = self.read_8_bytes(self.storage.PC + 2)
        self.valP = self.storage.PC + 18

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
        self.icode = self.read_half_byte(self.storage.PC + 0)
        self.ifun = self.read_half_byte(self.storage.PC + 1)
        self.valC = self.read_8_bytes(self.storage.PC + 2)
        self.valP = self.storage.PC + 18

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
        self.icode = self.read_half_byte(self.storage.PC + 0)
        self.ifun = self.read_half_byte(self.storage.PC + 1)
        self.valP = self.storage.PC + 4

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
    def Write_back(self):
        self.storage.State = self.storage.HLT


class nop(instr):
    def Fetch(self):
        self.valP = self.storage.PC + 2


def get_instr(dev: Devices) -> instr:
    tmp = int(dev.Mem[dev.PC], 16)
    choice = (halt, nop, rrmovq, irmovq, rmmovq, mrmovq, OPq, jXX, call, ret, pushq, popq)
    return choice[tmp](dev)


def run(dev: Devices):
    while dev.State == dev.AOK:
        ins = get_instr(dev)
        ins.Fetch()
        ins.Decode()
        ins.Execute()
        ins.Memory()
        ins.Write_back()
        ins.PC_update()
        print(dev)


if __name__ == '__main__':
    d = Devices()
    d.insert_code("30f8080000000000000000")
    run(d)
