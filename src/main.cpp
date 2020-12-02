//
// Created by 谢子飏 on 2020/12/02.
//

#include "Controller.h"
#include "infoList.h"

void Output(const Controller& c) {
    std::cout << "\n-------------------------------------------------\n" << std::endl;
    std::cout << "PC:\t0x" << std::hex << c.d.GetPC() << std::endl;
    std::cout << "State:\t" << StatList[c.d.Stat] << std::endl;

    std::cout << "CC:\t";
    for(int i = 0; i < 3; ++i)
        std::cout << CCList[i] << ": " << c.d.CFLAG[i] << "\t";

    std::cout << "\n\n";
    std::cout << "Register:" << std::endl;
    for(int i = 0; i < REG_SIZE; ++i)
    {
        std::cout << "\t" << RegList[i] << ":\t0x" << std::hex << c.d.Reg[i] << '\t';
        if(!(i & 1))
            std::cout << std::endl;
    }
}


int main() {
    Controller c;
    std::string raw_code;
    std::cin >> raw_code;
    c.FlashCode(raw_code);
    c.Run();
    Output(c);
    c.Reset();
}