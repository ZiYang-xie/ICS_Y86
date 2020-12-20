//
// Created by 王少文 on 2020/11/27.
//
#ifndef ICS_Y86_DEVICE_H
#define ICS_Y86_DEVICE_H
#include <cstdint>
#include <cstring>
#include <functional>
#include <queue>
#include <stack>
#include <string>

#include "index_const.h"
#include "pipeline_registers.h"
#include "../library/json.hpp"
const int MEM_SIZE = 0x1000;
const int CONSOLE_MEM_START = 0xf00;
const int CONSOLE_MEM_SIZE = 0x99;
const int GRAPHICS_MEM_FLAG = 0xdf8;
const int GRAPHICS_LENGTH = 8;
constexpr int GRAPHICS_MEM_SIZE = GRAPHICS_LENGTH*GRAPHICS_LENGTH*4;
constexpr int GRAPHICS_MEM_START = 0xe00;

const int REG_SIZE = 0xf;
class Device {
   public:
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
    void SetDSrcA();
    void SetDSrcB();
    uint64_t Reg[REG_SIZE]{};
    mutable uint8_t Mem[MEM_SIZE]{};
    //按ZF, SF, OF的顺序为0, 1, 2
    bool CFLAG[3]{true, false, false};
    uint8_t Stat{};
    //构造函数，str为写入内存的编码
    explicit Device(std::string s = "");
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
    [[nodiscard]] uint64_t GetPC() const;
    //取F的状态码
    void SetFControl();
    //取D的状态码
    void SetDControl();
    //取E的状态码
    void SetEControl();
    //计算set_cc,若为True,将e.CFLAG的内容写入CFLAG
    void SetCC();
    //更新if_jump_state状态机
    void UpdateIfJumpState();
    //读取Console的结果
    [[nodiscard]] std::string GetConsoleOutput() const;
    //读取Graphics的结果
    nlohmann::json GetGraphicsOutput() const;

   private:
    uint64_t bad_instr_num{0};

   public:
    uint64_t GetBadInstrNum() const;

   private:
    // 这是可执行的最后一个内存地址
    uint64_t text_section_end{0};
    // 这是一个硬件中并不存在的queue，用于方便我们在前端显示PC
    // 这个Addr的大小始终为5，因为我们总是显示在Writeback阶段的PC值
    std::queue<uint64_t> addr_queue;
    // 这是一个实际存在的硬件栈，用于缓存CALL的结果以更好的分支预测
    std::stack<uint64_t> hardware_stack;
    // 这是一个跳转的状态机(2-bit saturating counter)，工作原理如下
    // 如果是0和1，则不跳转；如果是2和3，则跳转
    // 如果成功跳转了一次，则加1；如果没有成功跳转，则减1
    uint8_t if_jump_state{2};
    // 设置F阶段的PredPC
    [[nodiscard]] uint64_t GetFPredPc();
    //判断地址是否合法
    static bool IfAddrReadable(uint64_t pos);
    //判断指令是否合法，后期增加指令时需要改
    static bool IfInstrValid(uint8_t icode);
    //选择ALU计算的函数
    std::function<uint64_t(uint64_t, uint64_t)> GetALUFunc(uint8_t ifun);
    //根据CFLAG返回cond
    [[nodiscard]] bool CalcCond(bool cflag[3]);
    // 判断是否是Load/Use Hazard
    [[nodiscard]] bool IfLoadUseH() const;
    // 判断是否是Mispredicted Branch
    [[nodiscard]] bool IfMispredicted() const;
    // 判断是否在处理ret
    [[nodiscard]] bool IfRet() const;
    [[nodiscard]] uint8_t SelectSrcA() const;
    [[nodiscard]] uint8_t SelectSrcB() const;
    [[nodiscard]] uint8_t SelectDstE();
    [[nodiscard]] uint8_t SelectDstM() const;
    [[nodiscard]] uint64_t SelectPC();
    [[nodiscard]] uint8_t SelectFStat() const;
    [[nodiscard]] uint64_t SelectAluA() const;
    [[nodiscard]] uint64_t SelectAluB() const;
    [[nodiscard]] bool IfAddrWriteable(uint64_t pos) const;
    [[nodiscard]] bool IfAddrExecutable(uint64_t pos) const;
    void Write4Bytes(uint64_t pos, uint64_t val);
    void Write2Bytes(uint64_t pos, uint64_t val);
    void Write1Bytes(uint64_t pos, uint64_t val);
    [[nodiscard]] uint64_t Read4Bytes(uint64_t pos) const;
    [[nodiscard]] uint64_t Read2Bytes(uint64_t pos) const;
    [[nodiscard]] uint64_t Read1Bytes(uint64_t pos) const;
    uint64_t ReadReg(uint8_t reg_idx, uint8_t ifun);
    void WriteReg(uint8_t reg_idx, uint8_t ifun, uint64_t val);
    void ReadMemToValM(uint64_t mem_addr);
    void WriteMemFromValA(uint64_t mem_addr);
#ifdef HARDWARE_STACK
    bool IfMispredictRet() const;
#endif
};

#endif  // ICS_Y86_DEVICE_H
