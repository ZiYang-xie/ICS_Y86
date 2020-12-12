//
// Created by 谢子飏 on 2020/12/02.
//

#include "Controller.h"
#include "Output.h"

int main() {
    Controller c;
    std::string raw_code;
    std::cin >> raw_code;
    c.FlashCode(raw_code);
    c.Run(0, false, 0);
    OutputFinal(c.d);
    c.Reset();
}