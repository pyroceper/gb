#include "util/bit_op.h"

namespace BitOperation
{
    void set_bit(uint16_t &val, int bit)
    {
        val = val | (1 << bit);
    }

    void clear_bit(uint16_t &val, int bit)
    {
        val = val & ~(1 << bit);
    }

    bool get_bit(uint16_t val, int bit)
    {
        return static_cast<bool>(val & (1 << bit));
    }

    void toggle_bit(uint16_t &val, int bit)
    {
        bool bit_val = get_bit(val, bit);
        if(bit_val)
            clear_bit(val, bit);
        else 
            set_bit(val, bit);
    }
}