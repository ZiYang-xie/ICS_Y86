#! /usr/bin/python3
import argparse
import copy


def get_hex_repr(h, size):
    size *= 2
    s = hex(h)[2:]
    res = ''
    if len(s) < size:
        s = (size - len(s)) * '0' + s
    for i in range(size - 2, -2, -2):
        res += s[i:i + 2]
    return res


def get_raw_str_repr(s):
    res = ''
    for c in s:
        res += f"{ord(c):x}"
    return res


def get_num(s: str):
    s = s.replace("$", "")
    res = 0
    if '0x' in s:
        res = int(s, 16)
    else:
        res = int(s)
    if res < 0:
        res += 2 ** 64
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
        "%r14": 14,
        "RNONE": 15
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
            res += get_hex_repr(self.imm, 8)
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
            'xorq': 3,
            'orq': 4,
            'shl': 5,
            'sal': 5,
            'shr': 6,
            'sar': 7,
            'mulq': 8,
            'divq': 9,
            'remq': 10,
            'minq': 11,
            'maxq': 12
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
        1 + 1


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


instr = {
    'halt': halt,
    'nop': nop,
    'rrmovq': rrmovq,
    'cmovle': rrmovq,
    'cmovl': rrmovq,
    'cmove': rrmovq,
    'cmovne': rrmovq,
    'cmovge': rrmovq,
    'cmovg': rrmovq,
    'irmovq': irmovq,
    'rmmovq': rmmovq,
    'mrmovq': mrmovq,
    'addq': OPq,
    'subq': OPq,
    'andq': OPq,
    'xorq': OPq,
    'orq': OPq,
    'shl': OPq,
    'sal': OPq,
    'shr': OPq,
    'sar': OPq,
    'mulq': OPq,
    'divq': OPq,
    'remq': OPq,
    'minq': OPq,
    'maxq': OPq,
    'iaddq': iOPq,
    'isubq': iOPq,
    'iandq': iOPq,
    'ixorq': iOPq,
    'jmp': jXX,
    'jle': jXX,
    'jl': jXX,
    'je': jXX,
    'jne': jXX,
    'jge': jXX,
    'jg': jXX,
    'call': call,
    'ret': ret,
    'pushq': pushq,
    'popq': popq
}


def gen_list(lines: list):
    res = []
    for line in lines:
        try:
            # 先处理无.string的情况
            if not ".string" in line:
                res.append(line.strip().replace(
                    '\t', ' ').replace(',', ' ').split(' '))
            else:
                split_list = line.split('"')
                res.append(split_list[0].strip().replace('\t', ' ').replace(',', ' ').split(
                    ' ') + [split_list[1]] + split_list[2].strip().replace('\t', ' ').replace(',', ' ').split(' '))
            res[-1] = [x for x in res[-1] if x != '']
        except Exception as e:
            print(f"Syntax Error At: {line}")
            print("Assemble Terminated")
            exit(1)
    return res


def remove_single_line_annot(lines: list):
    res = []
    for l in lines:
        try:
            if '#' in l or '//' in l:
                if '#' in l:
                    idx = l.index('#')
                if '//' in l:
                    idx = l.index('//') if l.index('//') < idx else idx
            else:
                idx = len(l)
            if idx != 0:
                res.append(l[0:idx])
        except Exception as e:
            print(f"Syntax Error At: {l}")
            print("Assemble Terminated")
            exit(1)
    return res


def remove_multi_line_annot(lines: list):
    res = []
    if_annot = False
    for line in lines:
        try:
            temp = []
            for c in line:
                if not if_annot:
                    if c != '/*':
                        temp.append(c)
                    else:
                        if_annot = True
                else:
                    if c == '*/':
                        if_annot = False
            if len(temp) != 0:
                res.append(temp)
        except Exception as e:
            print(f"Syntax Error At: {line}")
            print("Assemble Terminated")
            exit(1)
    return res


def detach_label(lines: list):
    res = []
    for line in lines:
        try:
            if ':' in line[0] and len(line) > 1:
                res.append([line[0]])
                res.append(line[1:])
            else:
                res.append(line)
        except Exception as e:
            print(f"Syntax Error At: {line}")
            print("Assemble Terminated")
            exit(1)
    return res


def get_memaddr(lines: list):
    res = [0]
    idx = 0
    for line in lines:
        try:
            idx += 1
            if line[0] == '.pos':
                res[-1] = int(line[1], 16)
                res.append(int(line[1], 16))
            elif line[0] == '.align':
                res[-1] = res[-1] - res[-1] % int(line[1]) + int(line[1])
                res.append(res[-1])
            elif line[0] == '.quad':
                res.append(res[idx - 1] + 8)
            elif line[0] == '.word':
                res.append(res[idx - 1] + 4)
            elif line[0] == '.hword':
                res.append(res[idx - 1] + 2)
            elif line[0] == '.byte':
                res.append(res[idx - 1] + 1)
            elif line[0] == '.string':
                res.append(res[idx - 1] + 2 * len(line[1]))
            elif ':' in line[0]:
                res.append(res[idx - 1])
            else:
                res.append(res[idx - 1] + instr[line[0]].size)
        except Exception as e:
            print(f"Syntax Error At: {line}")
            print("Assemble Terminated")
            exit(1)
    return res[:-1]


def replace_label(lines, mem):
    labels = {}
    res = []
    for i in range(len(lines)):
        if ':' in lines[i][0]:
            labels[lines[i][0].replace(':', '')] = '$' + hex(mem[i])
    for l in lines:
        temp = []
        for s in l:
            for la in labels.keys():
                if la == s and ':' not in s and '.' not in s:
                    s = s.replace(la, labels[la])
            temp.append(s)
        res.append(temp)
    return res


def gen_byte_code(lines):
    res = []
    for line in lines:
        try:
            if line[0] == '.pos' or line[0] == '.align' or ':' in line[0]:
                res.append('')
            elif line[0] == '.quad':
                res.append(get_hex_repr(int(line[1].replace("$", ""), 16), 8))
            elif line[0] == '.word':
                res.append(get_hex_repr(int(line[1].replace("$", ""), 16), 4))
            elif line[0] == '.hword':
                res.append(get_hex_repr(int(line[1].replace("$", ""), 16), 2))
            elif line[0] == '.byte':
                res.append(get_hex_repr(int(line[1].replace("$", ""), 16), 1))
            elif line[0] == '.string':
                res.append(get_raw_str_repr(line[1]))
            else:
                ins = instr[line[0]]
                if ins == halt:
                    temp = halt()
                elif ins == nop:
                    temp = nop()
                elif ins == rrmovq:
                    temp = rrmovq(line[0], line[1], line[2])
                elif ins == irmovq:
                    temp = irmovq(line[2], line[1])
                elif ins == rmmovq:
                    if '(' not in line[2] and ')' not in line[2]:
                        temp = rmmovq(line[1], "RNONE", line[2])
                    else:
                        opr = line[2].replace(')', '').split('(')
                        if len(opr[0]) == 0:
                            opr[0] = '0'
                        temp = rmmovq(line[1], opr[1], opr[0])
                elif ins == mrmovq:
                    if '(' not in line[1] and ')' not in line[1]:
                        temp = mrmovq("RNONE", line[1], line[2])
                    else:
                        opr = line[1].replace(')', '').split('(')
                        if len(opr[0]) == 0:
                            opr[0] = '0'
                        temp = mrmovq(opr[1], line[2], opr[0])
                elif ins == OPq:
                    temp = OPq(line[0], line[1], line[2])
                elif ins == iOPq:
                    temp = iOPq(line[0], line[2], line[1])
                elif ins == jXX:
                    temp = jXX(line[0], line[1])
                elif ins == call:
                    temp = call(line[1])
                elif ins == ret:
                    temp = ret()
                elif ins == pushq:
                    temp = pushq(line[1])
                elif ins == popq:
                    temp = popq(line[1])
                res.append(str(temp))
        except Exception as e:
            print(f"Syntax Error At: {line}")
            print("Assemble Terminated")
            exit(1)
    return res


def get_output(mem_addr, lines, byte_code):
    res = '\n'.join(
        [f"0x{mem_addr[i]:03x}: {byte_code[i]:24s}| {'' if ':' in lines[i][0] else '    '}{' '.join(lines[i])}" for i in
         range(len(lines))])
    return res


def process_to_yo(lines: list):
    lines = gen_list(lines)
    lines = remove_single_line_annot(lines)
    lines = remove_multi_line_annot(lines)
    lines = detach_label(lines)
    lines_ref = copy.deepcopy(lines)
    mem = get_memaddr(lines)
    lines = replace_label(lines, mem)
    byte_code = gen_byte_code(lines)
    return get_output(mem, lines_ref, byte_code)


def process_to_raw(lines: list, gen_prefix=True):
    last_instr_addr = 0
    res = ""
    for line in lines:
        if len(line) != 0:
            left_side = line.split(sep='|')[0].split(":")
            right_side = line.split(sep='|')[1]
            if len(left_side) == 2:
                res += "0" * (2 * int(left_side[0].strip(), 16) - len(res))
                res += left_side[1].strip()
                if '.' not in right_side and ':' not in right_side:
                    last_instr_addr = int(left_side[0], 16) + len(left_side[1].strip()) // 2
    if gen_prefix:
        return get_hex_repr(last_instr_addr, 8) + res
    else:
        return res


def assemble_file(filename):
    with open(filename) as f:
        return process_to_yo(f.readlines())


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Assemble .ys file to .yo file or/and .yo file to  raw byte file')
    parser.add_argument("sourcefile", help="The source file to be assembled")
    parser.add_argument("-o", "--output", dest='outfile',
                        action='store', help="Assign the output file")
    parser.add_argument("-r", "--raw", dest='raw',
                        action="store_true", help="Generate raw byte file")
    parser.add_argument("-np", "--noprefix", dest="noprefix",
                        action="store_true", help="Do not generate prefix in raw output")
    args = parser.parse_args()
    if args.sourcefile[-3:] == '.ys':
        res = assemble_file(args.sourcefile)
    elif args.sourcefile[-3:] == '.yo':
        args.raw = True
        with open(args.sourcefile) as f:
            res = f.read()
    else:
        print(
            f"Invalid file format {args.sourcefile[-3:]}, only support .yo and .ys file")
        exit(1)
    if args.raw is True:
        res = process_to_raw(res.split('\n'), args.noprefix is False)
    if args.outfile is None:
        print(res)
    else:
        with open(args.outfile, 'w')as f:
            f.write(res)
