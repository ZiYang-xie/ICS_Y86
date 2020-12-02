//
// Created by 谢子飏 on 2020/12/02.
//

#include "Controller.h"
#include "infoList.h"
#include <iostream>

using namespace std;

void Output(const Controller& c)
{
    cout << "\n-------------------------------------------------\n" << endl;
    cout << "PC:\t0x" << hex << c.d.GetPC() << endl;
    cout << "State:\t" << StatList[c.d.Stat] << endl;

    cout << "CC:\t";
    for(int i = 0; i < 3; ++i)
        cout << CCList[i] << ": " << c.d.CFLAG[i] << "\t";

    cout << "\n\n";
    cout << "Register:" << endl;
    for(int i = 0; i < 15; ++i)
    {
        cout << "\t" << RegList[i] << ":\t0x" << hex << c.d.Reg[i] << '\t';
        if(!(i & 1))
            cout << endl;
    }
}

int main()
{
    Controller c;
    string raw_code;
    cin >> raw_code;
    c.FlashCode(raw_code);
    c.Run();
    Output(c);
    c.Reset();
}