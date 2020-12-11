from instr import *
import sys
import copy
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
    return [list(filter(lambda x: x != '',
                        line.strip().replace('\t', ' ').replace(',', ' ').split(' '))) for line in lines]


def remove_single_line_annot(lines: list):
    res = []
    for l in lines:
        if '#' in l:
            idx = l.index('#')
        else:
            idx = len(l)
        if idx != 0:
            res.append(l[0:idx])
    return res


def remove_multi_line_annot(lines: list):
    res = []
    if_annot = False
    for line in lines:
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
    return res


def detach_label(lines: list):
    res = []
    for line in lines:
        if ':' in line[0] and len(line) > 1:
            res.append([line[0]])
            res.append(line[1:])
        else:
            res.append(line)
    return res


def get_memaddr(lines: list):
    res = [0]
    idx = 0
    for line in lines:
        idx += 1
        if line[0] == '.pos':
            res[-1] = int(line[1], 16)
            res.append(int(line[1], 16))
        elif line[0] == '.align':
            res[-1] = res[-1]-res[-1] % 8+8
            res.append(res[-1])
        elif line[0] == '.quad':
            res.append(res[idx-1]+8)
        elif line[0] == '.byte':
            res.append(res[idx-1]+2)
        elif ':' in line[0]:
            res.append(res[idx-1])
        else:
            res.append(res[idx-1]+instr[line[0]].size)
    return res[:-1]


def replace_label(lines, mem):
    labels = {}
    res = []
    for i in range(len(lines)):
        if ':' in lines[i][0]:
            labels[lines[i][0].replace(':', '')] = '$'+hex(mem[i])
    for l in lines:
        temp = []
        for s in l:
            for la in labels.keys():
                if la in s and not ':' in s and not '.' in s:
                    s = s.replace(la, labels[la])
            temp.append(s)
        res.append(temp)
    return res


def gen_byte_code(lines):
    res = []
    for line in lines:
        if line[0] == '.pos' or line[0] == '.align' or ':' in line[0]:
            res.append('')
        elif line[0] == '.quad':
            res.append(get_hex_repr(int(line[1].replace("$",""), 16)))
        elif line[0] == '.byte':
            res.append(hex(int(line[1],16))[2:])
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
                opr = line[2].replace(')', '').split('(')
                if len(opr[0]) == 0:
                    opr[0] = '0'
                temp = rmmovq(line[1], opr[1], opr[0])
            elif ins == mrmovq:
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
    return res


def output(mem_addr, lines, byte_code):
    for i in range(len(lines)):
        print(
            f"0x{mem_addr[i]:03x}: {byte_code[i]:24s}| {''if ':' in lines[i][0] else '    ' }{' '.join(lines[i])}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise ValueError("You can only input 1 file at a time")
    file_name = sys.argv[1]
    if file_name[-3:] != '.ys':
        print("Usage: python3 assembler.py file.ys")
        exit()
    res = ""
    with open(file_name) as f:
        try:
            lines = gen_list(f.readlines())
            lines = remove_single_line_annot(lines)
            lines = remove_multi_line_annot(lines)
            lines = detach_label(lines)
            lines_ref = copy.deepcopy(lines)
            mem = get_memaddr(lines)
            lines = replace_label(lines, mem)
            byte_code = gen_byte_code(lines)
            # print(lines)
            # print(list(map(hex,mem)))
            output(mem, lines_ref, byte_code)
        except Exception as e:
            print("Error in ", file_name, e, file=sys.stderr)
            print(lines, file=sys.stderr)
