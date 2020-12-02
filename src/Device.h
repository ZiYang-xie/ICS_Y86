//
// Created by 王少文 on 2020/11/27.
//
#include <cstdint>
#include <functional>
#include <string>

#include "pipeline_registers.h"
#ifndef ICS_Y86_DEVICE_H
#define ICS_Y86_DEVICE_H
const int MEM_SIZE = 0x1000;
const int RESERVED_SIZE = 0x100;  //用于后期扩展输出，作为保留内存不写入
const int REG_SIZE = 0xf;
class Device {
   private:
    // F,D,E,M,W为流水线寄存器
    // f,d,r,m,w为模拟导线而产生的变量，实际并不存在
    F_Reg F{};
    f_wire f{};
    D_Reg D{};
    d_wire d{};
    E_Reg E{};
    e_wire e{};
    M_Reg M{};
    m_wire m{};
    W_Reg W{};
    //更新predPC，该操作在Fetch阶段的最后执行
    void SetFPredPc();
    //判断地址是否合法
    static bool IfAddrValid(uint64_t pos);
    //判断指令是否合法，后期增加指令时需要改
    static bool IfInstrValid(uint8_t icode);

    //选择ALU计算的函数
    std::function<uint64_t(uint64_t, uint64_t)> GetALUFunc(uint8_t ifun);
    //根据CFLAG返回cond
    [[nodiscard]] bool cond() const;
    // 判断是否是Load/Use Hazard
    [[nodiscard]] bool IfLoadUseH() const;
    // 判断是否是Mispredicted Branch
    [[nodiscard]] bool IfMispredicted() const;
    // 判断是否在处理ret
    [[nodiscard]] bool IfRet() const;
    //取F的状态码
    [[nodiscard]] uint8_t GetFControl() const;
    //取D的状态码
    [[nodiscard]] uint8_t GetDControl() const;
    //取E的状态码
    [[nodiscard]] uint8_t GetEControl() const;

   public:
    const static uint8_t SAOK = 1;
    const static uint8_t SHLT = 2;
    const static uint8_t SADR = 3;
    const static uint8_t SINS = 4;
    const static uint8_t SBUB = 5;
    const static uint8_t CZF = 0;
    const static uint8_t CSF = 1;
    const static uint8_t COF = 2;
    // TODO: 来不及了，先把这个写成public的
    uint64_t Reg[REG_SIZE]{};
    uint8_t Mem[MEM_SIZE]{};
    bool CFLAG[3]{};  //按ZF, SF, OF的顺序为0, 1, 2
    uint8_t Stat{};
    //构造函数，str为写入内存的编码
    explicit Device(std::string s = "00");
    //执行Fetch阶段
    void Fetch();
    //执行Decode阶段
    void Decode();
    //执行Execute阶段
    void Execute();
    //执行Memory阶段
    void Memory();
    //执行Writeback阶段
    void Writeback();
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
    //从F中取值，放入D，模拟连线（实际上并不存在的过程)
    void F2D();
    //从D中取值，放入E，模拟连线
    void D2E();
    //从M中取值，放入W
    void E2M();
    //从M中取值，放入W
    void M2W();
    //返回程序结束时的PC值
    uint64_t GetPC() const;
};

#endif  // ICS_Y86_DEVICE_H
