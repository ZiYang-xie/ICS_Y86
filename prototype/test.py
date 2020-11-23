from controler import Controler
import unittest

class TestDevices(unittest.TestCase):
    def test_flash_code(self):
        c = Controler()
        c.flash_code("112233eeff000ff0")
        self.assertEqual(c.dev.Mem[0],17)
        self.assertEqual(c.dev.Mem[1],34)
        self.assertEqual(c.dev.Mem[2],51)
        self.assertEqual(c.dev.Mem[3],0xe*17)
        self.assertEqual(c.dev.Mem[4],0xf*17)
        self.assertEqual(c.dev.Mem[5],0)
        self.assertEqual(c.dev.Mem[6],0xf*16)
        self.assertEqual(c.dev.Mem[7],0xf)


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
        #TODO 这是个异常处理测试，我还没有开
        #c.flash_code("20f000")
        c.reset()
        c.flash_code("213000")
        c.dev.ZF = 1
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 5)
        c.reset()
        c.flash_code("213000")
        c.dev.ZF = 0
        c.dev.Reg[3] = 5
        c.run()
        self.assertEqual(c.dev.Reg[0], 0)



if __name__ == '__main__':
    unittest.main()
