#include <iostream>
#include <fmt/core.h>

#include "memory.h"
#include "cpu.h"
#include "util/register.h"


int main()
{
    fmt::print("Gameboy emulator\n");

    CPU cpu("tests/dmg_boot.bin");
    
    //temp
    // cpu.reg_pc = 0x100;
    // cpu.reg_sp = 0xFFFE;
    // cpu.reg_af = 0x1180;
    // cpu.reg_de = 0xFF56;
    // cpu.reg_hl = 0x000D;
    cpu.reg_sp = 0xFFFE;


    //temp
    char step {};
    uint8_t opcode {};

    step = 'c';
    while(step != 'e')
    {
        step = getchar();
        cpu.execute();
    }

    return 0;
}