def get_hex_repr(h):
    s = hex(h)[2:]
    res = ''
    if len(s) < 16:
        s = (16-len(s))*'0'+s
    for i in range(14, -2, -2):
        res += s[i:i+2]
    return res


def get_num(s: str):
    s = s.replace("$", "")
    res = 0
    if '0x' in s:
        res = int(s, 16)
    else:
        res = int(s)
    if res < 0:
        res += 2**64
    return res


def get_r(s):
    rchoice = {
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
    }
    return rchoice[s]


class Instr:
    def __init__(self):
        self.icode = None
        self.ifun = 0
        self.rA = None
        self.rB = None
        self.imm = None

    def __str__(self):
        res = hex(self.icode)[2:] + hex(self.ifun)[2:]
        if not self.rA is None and not self.rB is None:
            res += hex(self.rA)[2:] + hex(self.rB)[2:]
        if not self.imm is None:
            res += get_hex_repr(self.imm)
        return res


class halt(Instr):
    size = 1

    def __init__(self):
        super().__init__()
        self.icode = 0


class nop(Instr):
    size = 1

    def __init__(self):
        super().__init__()
        self.icode = 1


class rrmovq(Instr):
    size = 2

    def __init__(self, mov_str, ra, rb):
        super().__init__()
        self.icode = 2
        choice = {
            'rrmovq': 0,
            'cmovle': 1,
            'cmovl': 2,
            'cmove': 3,
            'cmovne': 4,
            'cmovge': 5,
            'cmovg': 6
        }
        self.ifun = choice[mov_str]
        self.rA = get_r(ra)
        self.rB = get_r(rb)


class irmovq(Instr):
    size = 10

    def __init__(self, rb, V):
        super().__init__()
        self.icode = 3
        self.rA = 0xf
        self.rB = get_r(rb)
        self.imm = get_num(V)


class rmmovq(Instr):
    size = 10

    def __init__(self, ra, rb, V):
        super().__init__()
        self.icode = 4
        self.rA = get_r(ra)
        self.rB = get_r(rb)
        self.imm = get_num(V)


class mrmovq(Instr):
    size = 10

    def __init__(self, ra, rb, D):
        super().__init__()
        self.icode = 5
        self.rA = get_r(rb)
        self.rB = get_r(ra)
        self.imm = get_num(D)


class OPq(Instr):
    size = 2

    def __init__(self, op_str, ra, rb):
        super().__init__()
        self.icode = 6
        choice = {
            'addq': 0,
            'subq': 1,
            'andq': 2,
            'xorq': 3
        }
        self.ifun = choice[op_str]
        self.rA = get_r(ra)
        self.rB = get_r(rb)





class jXX(Instr):
    size = 9

    def __init__(self, jXX_str, Dest):
        super().__init__()
        self.icode = 7
        choice = {'jmp': 0, 'jle': 1, 'jl': 2,
                  'je': 3, 'jne': 4, 'jge': 5, 'jg': 6}
        self.ifun = choice[jXX_str]
        self.imm = get_num(Dest)


class call(Instr):
    size = 9

    def __init__(self, Dest):
        super().__init__()
        self.icode = 8
        self.imm = get_num(Dest)


class ret(Instr):
    size = 1

    def __init__(self):
        super().__init__()
        self.icode = 9
        1+1


class pushq(Instr):
    size = 2

    def __init__(self, ra):
        super().__init__()
        self.icode = 0xa
        self.rA = get_r(ra)
        self.rB = 0xf


class popq(Instr):
    size = 2

    def __init__(self, ra):
        super().__init__()
        self.icode = 0xb
        self.rA = get_r(ra)
        self.rB = 0xf

class iOPq(Instr):
    size = 10

    def __init__(self, op_str, rb, imm):
        super().__init__()
        self.icode = 0xc
        choice = {
            'iaddq': 0,
            'isubq': 1,
            'iandq': 2,
            'ixorq': 3
        }
        self.ifun = choice[op_str]
        self.rA = 0xf
        self.rB = get_r(rb)
        self.imm = get_num(imm)