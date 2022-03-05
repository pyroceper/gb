#ifndef BIT_OP_H
#define BIT_OP_H

#include <cstdint>

namespace BitOperation
{
    void set_bit(uint16_t &val, int bit);
    void clear_bit(uint16_t &val, int bit);
    bool get_bit(uint16_t val, int bit);
    void toggle_bit(uint16_t &val, int bit);
}

#endif