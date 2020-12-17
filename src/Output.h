//
// Created by 谢子飏 on 2020/12/02.
//

#ifndef ICS_Y86_OUTPUT_H
#define ICS_Y86_OUTPUT_H
#include <array>
#include <iostream>
#include <string>
#include <vector>
#ifdef OUTPUT_JSON
#include "../library/json.hpp"
using json = nlohmann::json;
#endif
#include "Device.h"

// 返回Stat的编号所对应的名字，例如1对应NORMAL
std::string GetStatName(int idx);
// 返回CC的编号所对应的名字
std::string GetCCName(int idx);
// 返回Reg的编号所对应的名字
std::string GetRegName(int idx);
// 返回Control的编号所对应的名字
std::string GetControlName(int idx);
// 输出在运行过程中每一个Cycle的信息，包含了寄存器、Cflag和流水线寄存器的值
void OutputProcedure(std::ostream& os, int idx, const Device& d);
// 输出运行结束后的信息，包含PC、状态、寄存器和Cflag
void OutputFinal(const Device& d);
#ifdef OUTPUT_JSON
json OutputToJsonCycle(const Device& d);
void OutputToJsonFinal(int cycle_num, int ins_num,
                       const std::vector<json>& cycle, std::ostream& os);
#endif
#endif