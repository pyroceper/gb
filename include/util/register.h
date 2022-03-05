#ifndef REGISTER_H
#define REGISTER_H

#include <cstdint>

namespace Register 
{
    uint8_t get_register_low(uint16_t reg);
    uint8_t get_register_high(uint16_t reg);

    void set_register_low(uint16_t &reg, uint8_t value);
    void set_register_high(uint16_t &reg, uint8_t value);
}

#endif