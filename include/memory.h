#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <fstream>
#include <string>
#include <array>
#include <fmt/core.h>

class Memory
{
    public:
        void load_bootROM(const std::string &path);
        void load_ROM(const std::string &path);

        std::array<uint8_t, 0x100> bootrom;
        std::array<uint8_t, 0x10000> rom;
        
        bool unmap_bootrom {};

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
};

#endif