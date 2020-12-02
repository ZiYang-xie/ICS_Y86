//
// Created by 谢子飏 on 2020/12/02.
//

#ifndef ICS_Y86_OUTPUT_H
#define ICS_Y86_OUTPUT_H
#include "Controller.h"

#include <string>
#include <vector>
#include <iostream>

const static std::vector<std::string> StatList = {"None", "AOK", "HLT", "ADR", "INS"};
const static std::vector<std::string> CCList = {"ZF", "SF", "OF"};
const static std::vector<std::string> RegList = {"RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "None"};
const static std::vector<std::string> ControlList = {"Normal", "Bubble", "Stall"};

#endif