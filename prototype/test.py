import unittest

import numpy as np

from controler import Controler

RSP = 4


def reverse_str(s):
    if len(s) % 2 != 0:
        raise ValueError("s must have even length")
    if s == "":
        return ""
    else:
        return reverse_str(s[2:]) + s[0:2]


def str2int(s):
    return int(reverse_str(s), 16)


def int2str(n):
    s = hex(n)[2:]
    while len(s) < 16:
        s = '0' + s
    return reverse_str(s)


def write_8_bytes(dev, start_pos, val) -> None:
    while val > 0:
        dev.Mem[start_pos] = val & 0xff
        val = val >> 8
        start_pos += 1


def read_8_bytes(dev, start_pos) -> int:
    res = 0
    for i in range(8):
        res += dev.Mem[start_pos + i] << (8 * i)
    return np.uint64(res)


class TestDevices(unittest.TestCase):
    def test_flash_code(self):
        c = Controler()
        c.flash_code("112233eeff000ff0")
        self.assertEqual(c.dev.Mem[0], 17)
        self.assertEqual(c.dev.Mem[1], 34)
        self.assertEqual(c.dev.Mem[2], 51)
        self.assertEqual(c.dev.Mem[3], 0xe * 17)
        self.assertEqual(c.dev.Mem[4], 0xf * 17)
        self.assertEqual(c.dev.Mem[5], 0)
        self.assertEqual(c.dev.Mem[6], 0xf)
        self.assertEqual(c.dev.Mem[7], 0xf * 16)


class TestSingleInstr(unittest.TestCase):
    def test_halt(self):
        c = Controler()
        c.flash_code("00")
        c.run()
        self.assertEqual(c.dev.State, c.dev.HLT)
        self.assertEqual(c.dev.PC, 1)

    def test_nop(self):
        c = Controler()
        c.flash_code("101000")
        c.run()
        self.assertEqual(c.dev.State, c.dev.HLT)
        self.assertEqual(c.dev.PC, 3)

    def test_rrmovq(self):
        c = Controler()
        c.flash_code("203000")
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.State, c.dev.HLT)
        self.assertEqual(c.dev.PC, 3)
        self.assertEqual(c.dev.Reg[0], 5)
        # TODO 这是个异常处理测试，我还没有开，后同
        # c.flash_code("20f000")
        c.reset()
        c.flash_code("213000")
        c.dev.ZF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 5)
        c.reset()
        c.dev.ZF = 0
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 0)
        c.reset()
        c.dev.SF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 5)
        c.reset()
        c.dev.OF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 5)
        c.reset()
        c.dev.OF = 1
        c.dev.SF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 0)
        c.flash_code("223000")
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 0)
        c.reset()
        c.dev.ZF = 0
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 0)
        c.reset()
        c.dev.SF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 5)
        c.reset()
        c.dev.OF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 5)
        c.reset()
        c.dev.OF = 1
        c.dev.SF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 0)
        c.flash_code("233000")
        c.reset()
        c.dev.ZF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 5)
        c.reset()
        c.dev.ZF = 0
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 0)

    def test_irmovq(self):
        c = Controler()
        c.flash_code("30f4040000000000000000")
        c.run()
        self.assertEqual(c.dev.Reg[4], 4)
        c.flash_code("30f5ffffffffffffffff00")
        c.reset()
        c.run()
        self.assertEqual(c.dev.Reg[5], 0xffffffffffffffff)

    def test_rmmovq(self):
        c = Controler()
        c.flash_code("4012" + "0001000000000000")
        c.dev.Reg[1] = 0xdeadbeef
        c.run()
        self.assertEqual(read_8_bytes(c.dev, 0x100), 0xdeadbeef)
        c.flash_code("4012" + "0001000000000000")
        c.reset()
        c.dev.Reg[1] = 0xffffffffffffffff
        c.run()
        self.assertEqual(read_8_bytes(c.dev, 0x100), 0xffffffffffffffff)
        c.flash_code("4012" + "0001000000000000")
        c.reset()
        c.dev.Reg[1] = 0xdeadbeef
        c.dev.Reg[2] = 0x10
        c.run()
        self.assertEqual(read_8_bytes(c.dev, 0x110), 0xdeadbeef)

    def test_mrmovq(self):
        c = Controler()
        c.flash_code("5012" + "0001000000000000")
        write_8_bytes(c.dev, 0x100, 0xdeadbeef)
        c.run()
        self.assertEqual(c.dev.Reg[1], 0xdeadbeef)
        c.flash_code("5012" + "0001000000000000")
        c.reset()
        write_8_bytes(c.dev, 0x100, 0xffffffffffffffff)
        c.run()
        self.assertEqual(c.dev.Reg[1], 0xffffffffffffffff)
        c.flash_code("5012" + "0001000000000000")
        c.reset()
        write_8_bytes(c.dev, 0x110, 0xdeadbeef)
        c.dev.Reg[2] = 0x10
        c.run()
        self.assertEqual(c.dev.Reg[1], 0xdeadbeef)

    def test_OPq(self):
        c = Controler()
        c.flash_code("6012")
        c.reset()
        c.dev.Reg[1] = 0x10
        c.dev.Reg[2] = 0x03
        c.run()
        self.assertEqual(c.dev.Reg[2], 0x13)
        c.flash_code("6112")
        c.reset()
        c.dev.Reg[1] = 0x10
        c.dev.Reg[2] = 0x03
        c.run()
        self.assertEqual(c.dev.Reg[2], np.uint64(0x3 - 0x10))
        c.flash_code("6212")
        c.reset()
        c.dev.Reg[1] = 0xf
        c.dev.Reg[2] = 0x7
        c.run()
        self.assertEqual(c.dev.Reg[2], np.uint64(0x7))
        c.flash_code("6312")
        c.reset()
        c.dev.Reg[1] = 0xf
        c.dev.Reg[2] = 0x7
        c.run()
        self.assertEqual(c.dev.Reg[2], np.uint64(0x8))

    def test_jXX(self):
        c = Controler()
        c.flash_code("70" + int2str(0x100))
        c.run()
        self.assertEqual(c.dev.PC, 0x101)
        # 其他测试如果能在cmov过，应该在这里也能过
        c.reset()
        c.flash_code("71" + int2str(0x100))
        c.dev.ZF = 1
        c.run()
        self.assertEqual(c.dev.PC, 0x101)
        c.reset()
        c.run()
        self.assertEqual(c.dev.PC, 10)
        c.reset()
        c.dev.SF = 1
        c.run()
        self.assertEqual(c.dev.PC, 0x101)
        c.reset()
        c.dev.OF = 1
        c.run()
        self.assertEqual(c.dev.PC, 0x101)
        c.reset()
        c.dev.OF = 1
        c.dev.SF = 1
        c.run()
        self.assertEqual(c.dev.PC, 10)
        c.flash_code("72" + int2str(0x100))
        c.run()
        self.assertEqual(c.dev.PC, 10)
        c.reset()
        c.dev.ZF = 0
        c.run()
        self.assertEqual(c.dev.PC, 10)
        c.reset()
        c.dev.SF = 1
        c.run()
        self.assertEqual(c.dev.PC, 0x101)
        c.reset()
        c.dev.OF = 1
        c.run()
        self.assertEqual(c.dev.PC, 0x101)
        c.reset()
        c.dev.OF = 1
        c.dev.SF = 1
        c.run()
        self.assertEqual(c.dev.PC, 10)
        c.flash_code("73" + int2str(0x100))
        c.reset()
        c.dev.ZF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.PC, 0x101)
        c.reset()
        c.dev.ZF = 0
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.PC, 10)

    def test_call(self):
        c = Controler()
        c.flash_code("80" + int2str(0x100))
        c.dev.Reg[RSP] = 0x150
        c.run()
        self.assertEqual(c.dev.PC, 0x101)
        self.assertEqual(c.dev.Reg[RSP], 0x148)
        self.assertEqual(read_8_bytes(c.dev, 0x148), 0x9)

    def test_ret(self):
        c = Controler()
        c.flash_code("90001010")
        c.dev.Reg[RSP] = 0x148
        write_8_bytes(c.dev, 0x148, 0x3)
        c.run()
        self.assertEqual(c.dev.PC, 5)
        self.assertEqual(c.dev.Reg[RSP], 0x150)
        c.reset()
        c.dev.Reg[RSP] = 0x148
        write_8_bytes(c.dev, 0x148, 0x110)
        c.run()
        self.assertEqual(c.dev.PC, 0x111)
        self.assertEqual(c.dev.Reg[RSP], 0x150)

    def test_pushq(self):
        c = Controler()
        c.flash_code("a010")
        c.dev.Reg[1] = 0xdeadbeef
        c.dev.Reg[RSP] = 0x150
        c.run()
        self.assertEqual(c.dev.Reg[RSP], 0x148)
        self.assertEqual(read_8_bytes(c.dev, 0x148), 0xdeadbeef)

    def test_popq(self):
        c = Controler()
        c.flash_code("b010")
        c.dev.Reg[RSP] = 0x148
        write_8_bytes(c.dev,0x148,0xdeadbeef)
        c.run()
        self.assertEqual(c.dev.Reg[RSP], 0x150)
        self.assertEqual(c.dev.Reg[1],0xdeadbeef)


if __name__ == '__main__':
    unittest.main()
