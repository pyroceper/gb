#include "util/register.h"

namespace Register
{
    uint8_t get_register_high(uint16_t reg)
    {
        return (reg >> 8);
    }
    
    uint8_t get_register_low(uint16_t reg)
    {
        return (reg & 0x00FF);
    }

    void set_register_low(uint16_t &reg, uint8_t value)
    {
        reg = (reg & 0xFF00) | value;
    }
    void set_register_high(uint16_t &reg, uint8_t value)
    {
        reg = (reg & 0x00FF) | (value << 8);
    }
}
