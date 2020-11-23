from devices import Devices
import instr


class Controler:

    def __init__(self, dev = None):
        self.dev = dev
        if self.dev != None:
            return
        else:
            self.dev = Devices(size = 0x200)


    def get_instr(self) -> instr:
        tmp = self.dev.Mem[self.dev.PC] >> 4
        choice = (instr.halt, instr.nop, instr.rrmovq, instr.irmovq, instr.rmmovq, instr.mrmovq, instr.OPq, instr.jXX, instr.call, instr.ret, instr.pushq, instr.popq)
        return choice[tmp](self.dev)


    def run(self,debug = False):
        while self.dev.State == self.dev.AOK:
            ins = self.get_instr()
            ins.Fetch()
            ins.Decode()
            ins.Execute()
            ins.Memory()
            ins.Write_back()
            ins.PC_update()
            if debug:
                print(type(ins))
                print(self.dev)

    def flash_code(self,s):
        self.dev.str_memcpy(0,s)
    
    def reset(self):
        self.dev.Reg *= 0
        self.dev.ZF = 0
        self.dev.OF = 0
        self.dev.SF = 0
        self.dev.State = self.dev.AOK
        self.dev.PC = 0
