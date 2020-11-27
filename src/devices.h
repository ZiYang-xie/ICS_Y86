#include <cstdint>
const int MEM_SIZE = 0x1000;
const int REG_SIZE = 0x10;
struct Devices {
    uint8_t Mem[MEM_SIZE];
    uint64_t Reg[REG_SIZE];
    
};
