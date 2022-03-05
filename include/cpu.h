#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <cstdio>
#include <string>
#include <fmt/core.h>
#include <vector>

#include "memory.h"
#include "util/register.h"
#include "util/bit_op.h"
#include "util/logger.h"

class CPU
{
    public:
        CPU(const std::string &path);
        void execute();

        void interrupt_handler();

        uint16_t reg_pc {};
        uint16_t reg_sp {};

        int cycles {};

        Memory memory;

    //private:
        uint16_t reg_af {};
        uint16_t reg_bc {};
        uint16_t reg_de {};
        uint16_t reg_hl {};

        void set_reg_A(int val);
        void set_reg_B(int val);
        void set_reg_C(int val);
        void set_reg_D(int val);
        void set_reg_E(int val);
        void set_reg_H(int val);
        void set_reg_L(int val);

        uint8_t get_reg_A();
        uint8_t get_reg_B();
        uint8_t get_reg_C();
        uint8_t get_reg_D();
        uint8_t get_reg_E();
        uint8_t get_reg_H();
        uint8_t get_reg_L();
    private:
        // 7 6 5 4
        // Z N H C
        void set_carry_flag(bool val);
        void set_halfcarry_flag(bool val);
        void set_subtraction_flag(bool val);
        void set_zero_flag(bool val);

        bool get_carry_flag();   
        bool get_halfcarry_flag();
        bool get_subtraction_flag();
        bool get_zero_flag();     

        //uint8_t reg_flag;// Z N H C
        bool flag_z; // zero
        bool flag_n; // subtraction
        bool flag_h; // half carry
        bool flag_c; // carry

        bool halted;
        bool interrupt_enabled;

        //interrupt registers
        const uint16_t interrupt_enable = 0xFFFF;
        const uint16_t interrupt_flag = 0xFF0F;

        //interrupt vector
        const uint8_t iv_vblank = 0x40;
        const uint8_t iv_lcd_stat = 0x48;
        const uint8_t iv_timer = 0x50;
        const uint8_t iv_serial = 0x58;
        const uint8_t iv_joypad = 0x60;

        //instructions
        void ld_r8_u8(uint16_t &reg, bool is_high);
        void ld_r8_r8(uint16_t &reg1, bool is_high_reg1, uint16_t &reg2, bool is_high_reg2);
        void ld_r8_mHL(uint16_t &reg, bool is_high);
        void ld_mHL_r8(uint16_t &reg, bool is_high);
        void ld_mHL_u8();
        void ld_mHLi_A();
        void ld_A_mHLi();
        void ld_mHLd_A();
        void ld_A_mHLd();
        void ld_ffu8_A();
        void ld_ffC_A();
        void ld_A_ffu8();
        void ld_A_ffC();

        void ld_r16_u16(uint16_t &reg);
        void ld_mr16_A(uint16_t &reg);
        void ld_A_mr16(uint16_t &reg);
        void ld_mu16_A();
        void ld_A_mu16();

        void ld_SP_u16();
        void ld_HL_SPi8();
        void ld_SP_HL();

        void add_A_r8(uint16_t reg, bool is_high, bool carry);
        void add_A_mHL(bool carry);
        void add_A_u8(bool carry);
        void sub_A_r8(uint16_t reg, bool is_high, bool carry);
        void sub_A_mHL(bool carry);
        void sub_A_u8(bool carry);
        void and_A_r8(uint16_t reg, bool is_high);
        void and_A_mHL();
        void and_A_u8();
        void xor_A_r8(uint16_t reg, bool is_high);
        void xor_A_mHL();
        void xor_A_u8();
        void or_A_r8(uint16_t reg, bool is_high);
        void or_A_mHL();
        void or_A_u8();
        void cp_A_r8(uint16_t reg, bool is_high);
        void cp_A_mHL();
        void cp_A_u8();

        void add_HL_r16(uint16_t reg);
        void add_SP_i8();

        void inc_r8(uint16_t &reg, bool is_high);
        void dec_r8(uint16_t &reg, bool is_high);
        void inc_mHL();
        void dec_mHL();

        void inc_r16(uint16_t &reg);
        void dec_r16(uint16_t &reg);

        void ld_mu16_sp();

        void pop(uint16_t &reg);
        void push(uint16_t &reg);

        void ret();
        void ret_cc(bool cc);
        void reti();

        void jr_i8();
        void jr_cc_i8(bool cc);
        void jp_cc(bool cc);
        void jp();
        void jp_HL();

        void call();
        void call_cc(bool cc);
        void rst(uint8_t vec);

        void daa();
        void cpl();
        void scf();
        void ccf();

        void rlca();
        void rrca();
        void rla();
        void rra();
        void rlc_r8(uint16_t &reg, bool is_high);
        void rlc_mHL();
        void rrc_r8(uint16_t &reg, bool is_high);
        void rrc_mHL();
        void rl_r8(uint16_t &reg, bool is_high);
        void rl_mHL();
        void rr_r8(uint16_t &reg, bool is_high);
        void rr_mHL();
        void sla_r8(uint16_t &reg, bool is_high);
        void sla_mHL();
        void sra_r8(uint16_t &reg, bool is_high);
        void sra_mHL();
        void swap_r8(uint16_t &reg, bool is_high);
        void swap_mHL();
        void srl_r8(uint16_t &reg, bool is_high);
        void srl_mHL();

        void bit_r8(uint16_t &reg, bool is_high, int bit);
        void bit_mHL(int bit);
        void res_r8(uint16_t &reg, bool is_high, int bit);
        void res_mHL(int bit);
        void set_r8(uint16_t &reg, bool is_high, int bit);
        void set_mHL(int bit);

        void di();
        void ei();

        void stop();
        void halt();

         //helper
        uint8_t fetch_byte();
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
        void increment_cycle(int cycle = 1);      
        void cb_opcodes();
};


#endif