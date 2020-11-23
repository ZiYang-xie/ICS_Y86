import unittest
import main


class MyTestCase(unittest.TestCase):
    def test_str2int(self):
        s = "0900000000000000"
        self.assertEqual(0x9,main.instr.str2int(s))

    def test_int2str(self):
        n = 9
        s = "0900000000000000"
        self.assertEqual(s,main.instr.int2str(n))


if __name__ == '__main__':
    unittest.main()
