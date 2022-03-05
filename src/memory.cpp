#include "memory.h"

void Memory::load_bootROM(const std::string &path)
{
    std::ifstream bootrom_file(path.c_str(), std::ifstream::in | std::ifstream::binary);
    if(bootrom_file)
    {
        bootrom_file.seekg(0, bootrom_file.end);
        int bootrom_size = bootrom_file.tellg();
        bootrom_file.seekg(0, bootrom_file.beg);
        if(bootrom_size < 0 || bootrom_size > 0x100) 
        {
            fmt::print("Invalid BOOT ROM!\n");
            exit(1);
        }       
        bootrom.fill(0);
        bootrom_file.read(reinterpret_cast<char*>(&bootrom), bootrom_size);
        bootrom_file.close();
    }
    else 
    {
        fmt::print("Failed to load BOOT ROM!\n");
        exit(1);
    }

}

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
    //TODO FIX THIS
    if(address < 0x100)
        return bootrom[address];
    return rom[address];
}

void Memory::write(uint16_t address, uint8_t value)
{
    //TODO FIX THIS
    if((address & 0xFF) == 0x01)
        fmt::print("[0xFF01] = {0:#x}\n", value);
    if((address & 0xFF) == 0x02)
        fmt::print("[0xFF02] = {0:#x}\n", value);
    rom[address] = value;
}