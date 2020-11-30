//
// Created by 王少文 on 2020/11/27.
//
#include <cstdint>
#include <functional>
#include "pipeline_registers.h"
#ifndef ICS_Y86_DEVICE_H
#define ICS_Y86_DEVICE_H
const int MEM_SIZE = 0x1000;
const int RESERVED_SIZE = 0x100; //用于后期扩展输出，作为保留内存不写入
const int REG_SIZE = 0xf;
class Device {
    const static int SAOK = 1;
    const static int SHLT = 2;
    const static int SADR = 3;
    const static int SINS = 4;
    const static int SBUB = 5;
    uint64_t Reg[REG_SIZE];
    uint64_t Mem[MEM_SIZE];
    bool CFLAG[3];//按ZF, SF, OF的顺序为0, 1, 2
    uint8_t Stat;
    // F,D,E,M,W为流水线寄存器
    // f,d,r,m,w为模拟导线而产生的变量，实际并不存在
    F_Reg F;
    f_wire f;
    D_Reg D;
    d_wire d;
    E_Reg E;
    e_wire e;
    M_Reg M;
    m_wire m;
    W_Reg W;
    //读取在pos处的高4个比特
    //注意: 该函数不会检查pos的合法性
    [[nodiscard]] uint8_t ReadHigh4Bits(uint64_t pos) const;
    //读取在pos处的低4个比特
    //注意: 该函数不会检查pos的合法性
    [[nodiscard]] uint8_t ReadLow4Bits(uint64_t pos) const;
    //读取在pos处顺序8个字节
    //注意: 该函数不会检查pos的合法性
    [[nodiscard]] uint64_t Read8Bytes(uint64_t pos) const;
    //在pos出的8个字节写入val
    //注意: 该函数不会检查pos的合法性
    void Write8Bytes(uint64_t pos, uint64_t val);
    //更新predPC，该操作在Fetch阶段的最后执行
    void SetFPredPc();
    //判断地址是否合法
    static bool IfAddrValid(uint64_t pos);
    //判断指令是否合法，后期增加指令时需要改
    static bool IfInstrValid(uint8_t icode);
    //从F中取值，放入D，模拟连线（实际上并不存在的过程)
    void F2D();
    //从D中取值，放入E，模拟连线
    void D2E();
   std::function<int(int,int)> GetALUFunc(uint8_t ifun);
  public:
    //构造函数，str为写入内存的编码
    explicit Device(const char* str = nullptr);
    //执行Fetch阶段
    void Fetch();
    void Decode();
    void Execute();




};

#endif  // ICS_Y86_DEVICE_H
