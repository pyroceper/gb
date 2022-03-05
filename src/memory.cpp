#include "memory.h"

void Memory::load_ROM(const std::string &path)
{
    std::ifstream rom_file(path.c_str(), std::ifstream::in | std::ifstream::binary);

    if(rom_file)
    {
        rom_file.seekg(0, rom_file.end);
        int rom_size = rom_file.tellg();
        rom_file.seekg(0, rom_file.beg);
        
        fmt::print("ROM size: {}\n", rom_size);

        rom.fill(0);
        rom_file.read(reinterpret_cast<char*>(&rom), rom_size);

        rom_file.close();
    }
    else 
    {
        fmt::print("Failed to load ROM!\n");
        exit(1);
    }

}

uint8_t Memory::read(uint16_t address)
{
    return rom[address];
}

void Memory::write(uint16_t address, uint8_t value)
{
    if((address & 0xFF) == 0x01)
        fmt::print("[0xFF01] = {0:#x}\n", value);
    if((address & 0xFF) == 0x02)
        fmt::print("[0xFF02] = {0:#x}\n", value);
    rom[address] = value;
}