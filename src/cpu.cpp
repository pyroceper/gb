#include "cpu.h"

CPU::CPU(const std::string &path)
{
    memory.load_ROM(path);
    memory.write(0xFF44, 0x90);// ppu ly register
}

uint8_t CPU::fetch_byte() 
{
    increment_cycle();
    uint8_t byte = read(reg_pc);
    reg_pc++;
    
    return byte;
}

uint8_t CPU::read(uint16_t address)
{
    increment_cycle();
    return memory.read(address);
}

void CPU::write(uint16_t address, uint8_t value)
{
    increment_cycle();
    memory.write(address, value);
}

void CPU::increment_cycle(int cycle) 
{
    cycles += cycle * 4;
}

void CPU::execute()
{
    //fetch
    uint8_t opcode = fetch_byte();

    //temp
    fmt::print("\nPC: {0:#x}\n", reg_pc - 1);
    fmt::print("opcode: {0:#x}\n", opcode);
    fmt::print("A: {0:#x} B: {0:#x} C: {0:#x} D: {0:#x} E: {0:#x}\n", get_reg_A(), get_reg_B(), get_reg_C(), get_reg_D(), get_reg_E());
    fmt::print("H: {0:#x} L: {0:#x}\n", get_reg_H(), get_reg_L());
    fmt::print("SP: {0:#x}\n", reg_sp);

    switch(opcode)
    {
        case 0x00: Logger::print("NOP\n"); increment_cycle(); break; //NOP
        case 0x01: Logger::print("LD BC, u16\n"); ld_r16_u16(reg_bc); break; // LD BC, u16
        case 0x02: Logger::print("LD [BC], A\n"); ld_mr16_A(reg_bc); break; // LD [BC], A
        case 0x03: Logger::print("INC BC\n"); inc_r16(reg_bc); break; // INC BC
        case 0x04: Logger::print("INC B\n"); inc_r8(reg_bc, true); break; // INC B
        case 0x05: Logger::print("DEC B\n"); dec_r8(reg_bc, true); break; // DEC B
        case 0x06: Logger::print("LD B, u8\n"); ld_r8_u8(reg_bc, true); break; // LD B, u8
        case 0x07: Logger::print("RLCA \n"); rlca(); break; // RLCA
        case 0x08: Logger::print("LD [u16], SP\n"); ld_mu16_sp(); break; //LD [u16], SP
        case 0x09: Logger::print("ADD HL, BC\n"); add_HL_r16(reg_bc); break; // ADD HL, BC
        case 0x0A: Logger::print("LD A, [BC]\n"); ld_A_mr16(reg_bc); break; // LD A, [BC]
        case 0x0B: Logger::print("DEC BC\n"); dec_r16(reg_bc); break; // DEC BC
        case 0x0C: Logger::print("INC C\n"); inc_r8(reg_bc, false); break; // INC C
        case 0x0D: Logger::print("DEC C\n"); dec_r8(reg_bc, false); break; // DEC C
        case 0x0E: Logger::print("LD C, u8\n"); ld_r8_u8(reg_bc, false); break; // LD C, u8
        case 0x0F: Logger::print("RRCA \n"); rrca(); break; // RRCA
       
        case 0x10: Logger::print("STOP\n"); break; // STOP //TODO
        case 0x11: Logger::print("LD DE, u16\n"); ld_r16_u16(reg_de); break; // LD DE, u16
        case 0x12: Logger::print("LD [DE], A\n"); ld_mr16_A(reg_de); break; // LD [DE], A
        case 0x13: Logger::print("INC DE\n"); inc_r16(reg_de); break; // INC DE
        case 0x14: Logger::print("INC D\n"); inc_r8(reg_de, true); break; // INC D
        case 0x15: Logger::print("DEC D\n"); inc_r8(reg_de, true); break; // DEC D
        case 0x16: Logger::print("LD D, u8\n"); ld_r8_u8(reg_de, true); break; // LD D, u8
        case 0x17: Logger::print("RLA \n"); rla(); break; // RLA
        case 0x18: Logger::print("JR i8\n"); jr_i8(); break; // JR i8
        case 0x19: Logger::print("ADD HL, DE\n"); add_HL_r16(reg_de); break; // ADD HL, DE
        case 0x1A: Logger::print("LD A, [DE]\n"); ld_A_mr16(reg_de); break; // LD A, [DE]
        case 0x1B: Logger::print("DEC DE\n"); dec_r16(reg_de); break; // DEC DE
        case 0x1C: Logger::print("INC E\n"); inc_r8(reg_de, false); break; // INC E
        case 0x1D: Logger::print("DEC E\n"); inc_r8(reg_de, false); break; // DEC E
        case 0x1E: Logger::print("LD E, u8\n"); ld_r8_u8(reg_de, false); break; // LD E, u8
        case 0x1F: Logger::print("RRA\n"); rra(); break; // RRA

        case 0x20: Logger::print("JR NZ, i8\n"); jr_cc_i8(!get_zero_flag()); break; // JR NZ, i8
        case 0x21: Logger::print("LD HL, u16\n"); ld_r16_u16(reg_hl); break; // LD HL, u16
        case 0x22: Logger::print("LD [HL+], A\n"); ld_mHLi_A(); break; // LD [HL+], A
        case 0x23: Logger::print("INC HL\n"); inc_r16(reg_hl); break; // INC HL
        case 0x24: Logger::print("INC H\n"); inc_r8(reg_hl, true); break; // INC H
        case 0x25: Logger::print("DEC H\n"); dec_r8(reg_hl, true); break; // DEC H
        case 0x26: Logger::print("LD H, u8\n"); ld_r8_u8(reg_hl, true); break; // LD H, u8
        case 0x27: Logger::print("DAA\n"); daa(); break; // DAA
        case 0x28: Logger::print("JR Z, i8\n"); jr_cc_i8(get_zero_flag()); break; // JR Z, i8
        case 0x29: Logger::print("ADD HL, HL\n"); add_HL_r16(reg_hl); break; // ADD HL, HL
        case 0x2A: Logger::print("LD A, [HL+]\n"); ld_A_mHLi(); break; // LD A, [HL+]
        case 0x2B: Logger::print("DEC HL\n"); dec_r16(reg_hl); break; // DEC HL
        case 0x2C: Logger::print("INC L\n"); inc_r8(reg_hl, false); break; // INC L
        case 0x2D: Logger::print("DEC L\n"); dec_r8(reg_hl, false); break; // DEC L
        case 0x2E: Logger::print("LD L, u8\n"); ld_r8_u8(reg_hl, false); break;// LD L, u8
        case 0x2F: Logger::print("CPL\n"); cpl(); break; // CPL

        case 0x30: Logger::print("JR NC, i8\n"); jr_cc_i8(!get_carry_flag()); break; // JR NC, i8
        case 0x31: Logger::print("LD SP, u16\n"); ld_SP_u16(); break; // LD SP, u16
        case 0x32: Logger::print("LD [HL-], A\n"); ld_mHLd_A(); break; // LD [HL-], A
        case 0x33: Logger::print("INC SP\n"); inc_r16(reg_sp); break; // INC SP
        case 0x34: Logger::print("INC [HL]\n"); inc_mHL(); break; // INC [HL]
        case 0x35: Logger::print("DEC [HL]\n"); dec_mHL(); break; // DEC [HL]
        case 0x36: Logger::print("LD [HL], u8"); ld_mHL_u8(); break; // LD [HL], u8
        case 0x37: Logger::print("SCF\n"); scf(); break; // SCF
        case 0x38: Logger::print("JR C, i8\n"); jr_cc_i8(get_carry_flag()); break; // JR C, i8
        case 0x39: Logger::print("ADD HL SP\n"); add_HL_r16(reg_sp); break; // ADD HL, SP
        case 0x3A: Logger::print("LD A, [HL-]\n"); ld_A_mHLd(); break; // LD A, [HL-]
        case 0x3B: Logger::print("DEC SP\n"); dec_r16(reg_sp); break; // DEC SP
        case 0x3C: Logger::print("INC A\n"); inc_r8(reg_af, true); break; // INC A
        case 0x3D: Logger::print("DEC A\n"); dec_r8(reg_af, true); break; // DEC A
        case 0x3E: Logger::print("LD A, u8\n"); ld_r8_u8(reg_af, true); break; // LD A, u8
        case 0x3F: Logger::print("CCF \n"); ccf(); break; // CCF

        case 0x40: Logger::print("LD B, B\n"); ld_r8_r8(reg_bc, true, reg_bc, true); break; // LD B, B
        case 0x41: Logger::print("LD B, C\n"); ld_r8_r8(reg_bc, true, reg_bc, false); break; // LD B, C
        case 0x42: Logger::print("LD B, D\n"); ld_r8_r8(reg_bc, true, reg_de, true); break; // LD B, D
        case 0x43: Logger::print("LD B, E\n"); ld_r8_r8(reg_bc, true, reg_de, false);break; // LD B, E
        case 0x44: Logger::print("LD B, H\n"); ld_r8_r8(reg_bc, true, reg_hl, true); break; // LD B, H
        case 0x45: Logger::print("LD B, L\n"); ld_r8_r8(reg_bc, true, reg_hl, false); break; // LD B, L     
        case 0x46: Logger::print("LD B, [HL]\n"); ld_r8_mHL(reg_bc, true); break; // LD B, [HL]
        case 0x47: Logger::print("LD B, A\n"); ld_r8_r8(reg_bc, true, reg_af, true); break; // LD B, A
        case 0x48: Logger::print("LD C, B\n"); ld_r8_r8(reg_bc, false, reg_bc, true); break; // LD C, B
        case 0x49: Logger::print("LD C, C\n"); ld_r8_r8(reg_bc, false, reg_bc, false); break; // LD C, C
        case 0x4A: Logger::print("LD C, D\n"); ld_r8_r8(reg_bc, false, reg_de, true); break; // LD C, D
        case 0x4B: Logger::print("LD C, E\n"); ld_r8_r8(reg_bc, false, reg_de, false); break; // LD C, E
        case 0x4C: Logger::print("LD C, H\n"); ld_r8_r8(reg_bc, false, reg_hl, true); break; // LD C, H
        case 0x4D: Logger::print("LD C, L\n"); ld_r8_r8(reg_bc, false, reg_hl, false); break; // LD C, L
        case 0x4E: Logger::print("LD C, [HL]\n"); ld_r8_mHL(reg_bc, false); break; // LD C, [HL]
        case 0x4F: Logger::print("LD C, A\n"); ld_r8_r8(reg_bc, false, reg_af, true); break; // LD C, A

        case 0x50: Logger::print("LD D, B\n"); ld_r8_r8(reg_de, true, reg_bc, true); break; // LD D, B
        case 0x51: Logger::print("LD D, C\n"); ld_r8_r8(reg_de, true, reg_bc, false); break; // LD D, C
        case 0x52: Logger::print("LD D, D\n"); ld_r8_r8(reg_de, true, reg_de, true); break; // LD D, D
        case 0x53: Logger::print("LD D, E\n"); ld_r8_r8(reg_de, true, reg_de, false); break; // LD D, E
        case 0x54: Logger::print("LD D, H\n"); ld_r8_r8(reg_de, true, reg_hl, true); break; // LD D, H
        case 0x55: Logger::print("LD D, L\n"); ld_r8_r8(reg_de, true, reg_hl, false); break; // LD D, L     
        case 0x56: Logger::print("LD D, [HL]\n"); ld_r8_mHL(reg_de, true); break; // LD D, [HL]
        case 0x57: Logger::print("LD D, A\n"); ld_r8_r8(reg_de, true, reg_af, true); break; // LD D, A
        case 0x58: Logger::print("LD E, B\n"); ld_r8_r8(reg_de, false, reg_bc, true); break; // LD E, B
        case 0x59: Logger::print("LD E, C\n"); ld_r8_r8(reg_de, false, reg_bc, false); break; // LD E, C
        case 0x5A: Logger::print("LD E, D\n"); ld_r8_r8(reg_de, false, reg_de, true); break; // LD E, D
        case 0x5B: Logger::print("LD E, E\n"); ld_r8_r8(reg_de, false, reg_de, false); break; // LD E, E
        case 0x5C: Logger::print("LD E, H\n"); ld_r8_r8(reg_de, false, reg_hl, true); break; // LD E, H
        case 0x5D: Logger::print("LD E, L\n"); ld_r8_r8(reg_de, false, reg_hl, false); break; // LD E, L
        case 0x5E: Logger::print("LD E, [HL]\n"); ld_r8_mHL(reg_de, false); break; // LD E, [HL]
        case 0x5F: Logger::print("LD E, A\n"); ld_r8_r8(reg_de, false, reg_af, true); break; // LD E, A

        case 0x60: Logger::print("LD H, B\n"); ld_r8_r8(reg_hl, true, reg_bc, true); break; // LD H, B
        case 0x61: Logger::print("LD H, C\n"); ld_r8_r8(reg_hl, true, reg_bc, false); break; // LD H, C
        case 0x62: Logger::print("LD H, D\n"); ld_r8_r8(reg_hl, true, reg_de, true); break; // LD H, D
        case 0x63: Logger::print("LD H, E\n"); ld_r8_r8(reg_hl, true, reg_de, false); break; // LD H, E
        case 0x64: Logger::print("LD H, H\n"); ld_r8_r8(reg_hl, true, reg_hl, true); break; // LD H, H
        case 0x65: Logger::print("LD H, L\n"); ld_r8_r8(reg_hl, true, reg_hl, false); break; // LD H, L     
        case 0x66: Logger::print("LD H, [HL]\n"); ld_r8_mHL(reg_hl, true); break; // LD H, [HL]
        case 0x67: Logger::print("LD H, A\n"); ld_r8_r8(reg_hl, true, reg_af, true); break; // LD H, A
        case 0x68: Logger::print("LD L, B\n"); ld_r8_r8(reg_hl, false, reg_bc, true); break; // LD L, B
        case 0x69: Logger::print("LD L, C\n"); ld_r8_r8(reg_hl, false, reg_bc, false); break; // LD L, C
        case 0x6A: Logger::print("LD L, D\n"); ld_r8_r8(reg_hl, false, reg_de, true); break; // LD L, D
        case 0x6B: Logger::print("LD L, E\n"); ld_r8_r8(reg_hl, false, reg_de, false); break; // LD L, E
        case 0x6C: Logger::print("LD L, H\n"); ld_r8_r8(reg_hl, false, reg_hl, true); break; // LD L, H
        case 0x6D: Logger::print("LD L, L\n"); ld_r8_r8(reg_hl, false, reg_hl, false); break; // LD L, L
        case 0x6E: Logger::print("LD L, [HL]\n"); ld_r8_mHL(reg_hl, false); break; // LD L, [HL]
        case 0x6F: Logger::print("LD L, A\n"); ld_r8_r8(reg_hl, false, reg_af, true); break; // LD L, A

        case 0x70: Logger::print("LD [HL], B\n"); ld_mHL_r8(reg_bc, true); break; // LD [HL], B
        case 0x71: Logger::print("LD [HL], C\n"); ld_mHL_r8(reg_bc, false); break; // LD [HL], C
        case 0x72: Logger::print("LD [HL], D\n"); ld_mHL_r8(reg_de, true); break; // LD [HL], D
        case 0x73: Logger::print("LD [HL], E\n"); ld_mHL_r8(reg_de, false); break; // LD [HL], E
        case 0x74: Logger::print("LD [HL], H\n"); ld_mHL_r8(reg_hl, true); break; // LD [HL], H
        case 0x75: Logger::print("LD [HL], L\n"); ld_mHL_r8(reg_hl, false); break; // LD [HL], L
        case 0x76: Logger::print("HALT\n"); halt(); break; // HALT
        case 0x77: Logger::print("LD [HL], A\n"); ld_mHL_r8(reg_af, true); break; // LD [HL], A
        case 0x78: Logger::print("LD A, B\n"); ld_r8_r8(reg_af, true, reg_bc, true); break; // LD A, B
        case 0x79: Logger::print("LD A, C\n"); ld_r8_r8(reg_af, true, reg_bc, false); break; // LD A, C
        case 0x7A: Logger::print("LD A, D\n"); ld_r8_r8(reg_af, true, reg_de, true); break; // LD A, D
        case 0x7B: Logger::print("LD A, E\n"); ld_r8_r8(reg_af, true, reg_de, false); break; // LD A, E
        case 0x7C: Logger::print("LD A, H\n"); ld_r8_r8(reg_af, true, reg_hl, true); break; // LD A, H
        case 0x7D: Logger::print("LD A, L\n"); ld_r8_r8(reg_af, true, reg_hl, false); break; // LD A, L 
        case 0x7E: Logger::print("LD A, [HL]\n"); ld_r8_mHL(reg_af, true); break; // LD A, [HL]
        case 0x7F: Logger::print("LD A, A\n"); ld_r8_r8(reg_af, true, reg_af, true); break; // LD A, A 

        case 0x80: Logger::print("ADD A, B\n"); add_A_r8(reg_bc, true, false); break; // ADD A, B
        case 0x81: Logger::print("ADD A, C\n"); add_A_r8(reg_bc, false, false); break; // ADD A, C
        case 0x82: Logger::print("ADD A, D\n"); add_A_r8(reg_de, true, false); break; // ADD A, D
        case 0x83: Logger::print("ADD A, E\n"); add_A_r8(reg_de, false, false); break; // ADD A, E
        case 0x84: Logger::print("ADD A, H\n"); add_A_r8(reg_hl, true, false); break; // ADD A, H
        case 0x85: Logger::print("ADD A, L\n"); add_A_r8(reg_hl, false, false); break; // ADD A, L
        case 0x86: Logger::print("ADD A, [HL]\n"); add_A_mHL(false); break; // ADD A, [HL]
        case 0x87: Logger::print("ADD A, A\n"); add_A_r8(reg_af, true, false); break; // ADD A, A
        case 0x88: Logger::print("ADC A, B\n"); add_A_r8(reg_bc, true, true); break; // ADC A, B
        case 0x89: Logger::print("ADC A, C\n"); add_A_r8(reg_bc, false, true); break; // ADC A, C
        case 0x8A: Logger::print("ADC A, D\n"); add_A_r8(reg_de, true, true); break; // ADC A, D
        case 0x8B: Logger::print("ADC A, E\n"); add_A_r8(reg_de, false, true); break; // ADC A, E
        case 0x8C: Logger::print("ADC A, H\n"); add_A_r8(reg_hl, true, true); break; // ADC A, H
        case 0x8D: Logger::print("ADC A, L\n"); add_A_r8(reg_hl, false, true); break; // ADC A, L
        case 0x8E: Logger::print("ADC A, [HL]\n"); add_A_mHL(true); break; // ADC A, [HL]
        case 0x8F: Logger::print("ADC A, A\n"); add_A_r8(reg_af, true, true); break; // ADC A, A

        case 0x90: Logger::print("SUB A, B\n"); sub_A_r8(reg_bc, true, false); break; // SUB A, B
        case 0x91: Logger::print("SUB A, C\n"); sub_A_r8(reg_bc, false, false); break; // SUB A, C
        case 0x92: Logger::print("SUB A, D\n"); sub_A_r8(reg_de, true, false); break; // SUB A, D
        case 0x93: Logger::print("SUB A, E\n"); sub_A_r8(reg_de, false, false); break; // SUB A, E
        case 0x94: Logger::print("SUB A, H\n"); sub_A_r8(reg_hl, true, false); break; // SUB A, H
        case 0x95: Logger::print("SUB A, L\n"); sub_A_r8(reg_hl, false, false); break; // SUB A, L
        case 0x96: Logger::print("SUB A, [HL]\n"); sub_A_mHL(false); break; // SUB A, [HL]
        case 0x97: Logger::print("SUB A, A\n"); sub_A_r8(reg_af, true, false); break; // SUB A, A
        case 0x98: Logger::print("SBC A, B\n"); sub_A_r8(reg_bc, true, true); break; // SBC A, B
        case 0x99: Logger::print("SBC A, C\n"); sub_A_r8(reg_bc, false, true); break; // SBC A, C
        case 0x9A: Logger::print("SBC A, D\n"); sub_A_r8(reg_de, true, true); break; // SBC A, D
        case 0x9B: Logger::print("SBC A, E\n"); sub_A_r8(reg_de, false, true); break; // SBC A, E
        case 0x9C: Logger::print("SBC A, H\n"); sub_A_r8(reg_hl, true, true); break; // SBC A, H
        case 0x9D: Logger::print("SBC A, L\n"); sub_A_r8(reg_hl, false, true); break; // SBC A, L
        case 0x9E: Logger::print("SBC A, [HL]\n"); sub_A_mHL(true); break; // SBC A, [HL]
        case 0x9F: Logger::print("SBC A, A\n"); sub_A_r8(reg_af, true, true); break; // SBC A, A

        case 0xA0: Logger::print("AND A, B\n"); and_A_r8(reg_bc, true); break; // AND A, B
        case 0xA1: Logger::print("AND A, C\n"); and_A_r8(reg_bc, false); break; // AND A, C
        case 0xA2: Logger::print("AND A, D\n"); and_A_r8(reg_de, true); break; // AND A, D
        case 0xA3: Logger::print("AND A, E\n"); and_A_r8(reg_de, false); break; // AND A, E
        case 0xA4: Logger::print("AND A, H\n"); and_A_r8(reg_hl, true); break; // AND A, H
        case 0xA5: Logger::print("AND A, L\n"); and_A_r8(reg_hl, false); break; // AND A, L
        case 0xA6: Logger::print("AND A, [HL]\n"); and_A_mHL(); break; // AND A, [HL]
        case 0xA7: Logger::print("AND A, A\n"); and_A_r8(reg_af, true); break; // AND A, A
        case 0xA8: Logger::print("XOR A, B\n"); xor_A_r8(reg_bc, true); break; // XOR A, B
        case 0xA9: Logger::print("XOR A, C\n"); xor_A_r8(reg_bc, false); break; // XOR A, C
        case 0xAA: Logger::print("XOR A, D\n"); xor_A_r8(reg_de, true); break; // XOR A, D
        case 0xAB: Logger::print("XOR A, E\n"); xor_A_r8(reg_de, false); break; // XOR A, E
        case 0xAC: Logger::print("XOR A, H\n"); xor_A_r8(reg_hl, true); break; // XOR A, H
        case 0xAD: Logger::print("XOR A, L\n"); xor_A_r8(reg_hl, false); break; // XOR A, L
        case 0xAE: Logger::print("XOR A, [HL]\n"); xor_A_mHL(); break; // XOR A, [HL]
        case 0xAF: Logger::print("XOR A, A\n"); xor_A_r8(reg_af, true); break; // XOR A, A

        case 0xB0: Logger::print("OR A, B\n"); or_A_r8(reg_bc, true); break; // OR A, B
        case 0xB1: Logger::print("OR A, C\n"); or_A_r8(reg_bc, false); break; // OR A, C
        case 0xB2: Logger::print("OR A, D\n"); or_A_r8(reg_de, true); break; // OR A, D
        case 0xB3: Logger::print("OR A, E\n"); or_A_r8(reg_de, false); break; // OR A, E
        case 0xB4: Logger::print("OR A, H\n"); or_A_r8(reg_hl, true); break; // OR A, H
        case 0xB5: Logger::print("OR A, L\n"); or_A_r8(reg_hl, false); break; // OR A, L
        case 0xB6: Logger::print("OR A, [HL]\n"); or_A_mHL(); break; // OR A, [HL]
        case 0xB7: Logger::print("OR A, A\n"); or_A_r8(reg_af, true); break; // OR A, A
        case 0xB8: Logger::print("CP A, B\n"); cp_A_r8(reg_bc, true); break; // CP A, B
        case 0xB9: Logger::print("CP A, C\n"); cp_A_r8(reg_bc, false); break; // CP A, C
        case 0xBA: Logger::print("CP A, D\n"); cp_A_r8(reg_de, true); break; // CP A, D
        case 0xBB: Logger::print("CP A, E\n"); cp_A_r8(reg_de, false); break; // CP A, E
        case 0xBC: Logger::print("CP A, H\n"); cp_A_r8(reg_hl, true); break; // CP A, H
        case 0xBD: Logger::print("CP A, L\n"); cp_A_r8(reg_hl, false); break; // CP A, L
        case 0xBE: Logger::print("CP A, [HL]\n"); cp_A_mHL(); break; // CP A, [HL]
        case 0xBF: Logger::print("CP A, A\n"); cp_A_r8(reg_af, true); break; // CP A, A

        case 0xC0: Logger::print("RET NZ\n"); ret_cc(!get_zero_flag()); break; // RET NZ
        case 0xC1: Logger::print("POP BC\n"); pop(reg_bc); break; // POP BC
        case 0xC2: Logger::print("JP NZ, u16\n"); jp_cc(!get_zero_flag()); break; // JP NZ, u16
        case 0xC3: Logger::print("JP u16\n"); jp(); break; // JP u16
        case 0xC4: Logger::print("CALL cc, u16\n"); call_cc(!get_zero_flag()); break; // CALL cc, u16
        case 0xC5: Logger::print("PUSH BC\n"); push(reg_bc); break; // PUSH BC
        case 0xC6: Logger::print("ADD A, u8\n"); add_A_u8(false); break; // ADD A, u8
        case 0xC7: Logger::print("RST 00h\n"); rst(0x00); break; // RST 00h
        case 0xC8: Logger::print("RET Z\n"); ret_cc(get_zero_flag()); break; // RET Z
        case 0xC9: Logger::print("RET\n"); ret(); break; // RET
        case 0xCA: Logger::print("JP Z, u16"); jp_cc(get_zero_flag()); break; // JP Z, u16
        case 0xCB: Logger::print("PREFIX CB\n"); cb_opcodes(); break; // prefix CB
        case 0xCC: Logger::print("CALL Z, u16\n"); call_cc(get_zero_flag()); break; // CALL Z, u16
        case 0xCD: Logger::print("CALL u16\n"); call(); break; // CALL u16
        case 0xCE: Logger::print("ADC A, u8\n"); add_A_u8(true); break; // ADC A, u8
        case 0xCF: Logger::print("RST 08h\n"); rst(0x08); break; // RST 08h

        case 0xD0: Logger::print("RET NC\n"); ret_cc(!get_carry_flag()); break; // RET NC
        case 0xD1: Logger::print("POP DE\n"); pop(reg_de); break; // POP DE
        case 0xD2: Logger::print("JP NC, u16\n"); jp_cc(!get_carry_flag()); break; // JP NC, u16

        case 0xD4: Logger::print("CALL NC, u16 \n"); call_cc(!get_carry_flag()); break; // CALL NC, u16 
        case 0xD5: Logger::print("PUSH DE\n"); push(reg_de); break; // PUSH DE
        case 0xD6: Logger::print("SUB A, u8\n"); sub_A_u8(false); break; // SUB A, u8
        case 0xD7: Logger::print("RST 10h\n"); rst(0x10); break; // RST 10h
        case 0xD8: Logger::print("RET C\n"); ret_cc(get_carry_flag()); break; // RET C
        case 0xD9: Logger::print("RETI\n"); reti(); break; // RETI
        case 0xDA: Logger::print("JP Z, u16\n"); jp_cc(get_carry_flag()); break; // JP C, u16

        case 0xDC: Logger::print("CALL C, u16\n"); call_cc(get_carry_flag()); break; // CALL C, u16

        case 0xDE: Logger::print("SBC A, u8\n"); sub_A_u8(true); break; // SBC A, u8

        case 0xDF: Logger::print("RST 18h\n"); rst(0x18); break; // RST 18h

        case 0xE0: Logger::print("LD [FF00+u8], A\n"); ld_ffu8_A(); break; // LD (FF00+u8), A
        case 0xE1: Logger::print("POP HL\n"); pop(reg_hl); break; // POP HL
        case 0xE2: Logger::print("LD [FF00+C], A"); ld_ffC_A(); break; // LD (FF00+C), A

        case 0xE5: Logger::print("PUSH HL\n"); push(reg_hl); break; // PUSH HL
        case 0xE6: Logger::print("AND A, u8\n"); and_A_u8(); break; // AND A, u8
        case 0xE7: Logger::print("RST 20h\n"); rst(0x20); break; // RST 20h
        case 0xE8: Logger::print("ADD SP, i8\n"); add_SP_i8(); break; // ADD SP, i8
        case 0xE9: Logger::print("JP HL\n"); jp_HL(); break; // JP HL
        case 0xEA: Logger::print("LD [u16], A\n"); ld_mu16_A(); break; // LD [u16], A
        
        case 0xEE: Logger::print("XOR A, u8\n"); xor_A_u8(); break; // XOR A, u8
        case 0xEF: Logger::print("RST 28h\n"); rst(0x28); break; // RST 28h

        case 0xF0: Logger::print("LD A, [FF00+u8]\n"); ld_A_ffu8(); break; // LD A, (FF00+u8)
        case 0xF1: Logger::print("POP AF\n"); pop(reg_af); break; // POP AF
        case 0xF2: Logger::print("LD A, [FF00+c]"); break; // LD A, (FF00+C)
        case 0xF3: Logger::print("DI\n"); di(); break; // DI

        case 0xF5: Logger::print("PUSH AF\n"); push(reg_af); break; // PUSH AF
        case 0xF6: Logger::print("OR A, u8\n"); or_A_u8(); break; // OR A, u8
        case 0xF7: Logger::print("RST 30h"); rst(0x30); break; // RST 30h
        case 0xF8: Logger::print("LD HL, SP+i8\n"); ld_HL_SPi8(); break; // LD HL, SP+i8
        case 0xF9: Logger::print("LD SP, HL\n"); ld_SP_HL(); break; // LD SP, HL
        case 0xFA: Logger::print("LD A, (u16)\n"); ld_A_mu16(); break; // LD A, (u16)
        case 0xFB: Logger::print("EI\n"); ei(); break; // EI

        case 0xFE: Logger::print("CP A, u8\n"); cp_A_u8(); break; // CP A, u8
        case 0xFF: Logger::print("RST 38h"); rst(0x38); break; // RST 38h

        default: fmt::print("UKNOWN OPCODE\n");
    }
}

void CPU::cb_opcodes()
{
    //fetch
    uint8_t opcode = fetch_byte();
    //fmt::print("opcode: {0:#x}\n", opcode);

    switch(opcode)
    {
        case 0x00: Logger::print("RLC B\n"); rlc_r8(reg_bc, true); break; // RLC B
        case 0x01: Logger::print("RLC C\n"); rlc_r8(reg_bc, false); break; // RLC C
        case 0x02: Logger::print("RLC D\n"); rlc_r8(reg_de, true); break; // RLC D
        case 0x03: Logger::print("RLC E\n"); rlc_r8(reg_de, false); break; // RLC E
        case 0x04: Logger::print("RLC H\n"); rlc_r8(reg_hl, true); break; // RLC H
        case 0x05: Logger::print("RLC L\n"); rlc_r8(reg_hl, false); break; // RLC L
        case 0x06: Logger::print("RLC [HL]\n"); rlc_mHL(); break; // RLC [HL]
        case 0x07: Logger::print("RLC A\n"); rrc_r8(reg_af, true); break; // RLC A
        case 0x08: Logger::print("RRC B\n"); rrc_r8(reg_bc, true); break; // RRC B
        case 0x09: Logger::print("RRC C\n"); rrc_r8(reg_bc, false); break; // RRC C
        case 0x0A: Logger::print("RRC D\n"); rrc_r8(reg_de, true); break; // RRC D
        case 0x0B: Logger::print("RRC E\n"); rrc_r8(reg_de, false); break; // RRC E
        case 0x0C: Logger::print("RRC H\n"); rrc_r8(reg_hl, true); break; // RRC H
        case 0x0D: Logger::print("RRC L\n"); rrc_r8(reg_hl, false); break; // RRC L
        case 0x0E: Logger::print("RRC [HL]\n"); rrc_mHL(); break; // RRC [HL]
        case 0x0F: Logger::print("RRC A\n"); rrc_r8(reg_af, true); break; // RRC A

        case 0x10: Logger::print("RL B\n"); rl_r8(reg_bc, true); break; // RL B
        case 0x11: Logger::print("RL C\n"); rl_r8(reg_bc, false); break; // RL C
        case 0x12: Logger::print("RL D\n"); rl_r8(reg_de, true); break; // RL D
        case 0x13: Logger::print("RL E\n"); rl_r8(reg_de, false); break; // RL E
        case 0x14: Logger::print("RL H\n"); rl_r8(reg_hl, true); break; // RL H
        case 0x15: Logger::print("RL L\n"); rl_r8(reg_hl, false); break; // RL L
        case 0x16: Logger::print("RL [HL]\n"); rl_mHL(); break; // RL [HL]
        case 0x17: Logger::print("RL A\n"); rl_r8(reg_af, true); break; // RL A
        case 0x18: Logger::print("RR B\n"); rr_r8(reg_bc, true); break; // RR B
        case 0x19: Logger::print("RR C\n"); rr_r8(reg_bc, false); break; // RR C
        case 0x1A: Logger::print("RR D\n"); rr_r8(reg_de, true); break; // RR D
        case 0x1B: Logger::print("RR E\n"); rr_r8(reg_de, false); break; // RR E
        case 0x1C: Logger::print("RR H\n"); rr_r8(reg_hl, true); break; // RR H
        case 0x1D: Logger::print("RR L\n"); rr_r8(reg_hl, false); break; // RR L
        case 0x1E: Logger::print("RR [HL]\n"); rr_mHL(); break; // RR [HL]
        case 0x1F: Logger::print("RR A\n"); rr_r8(reg_af, true); break; // RR A

        case 0x20: Logger::print("SLA B\n"); sla_r8(reg_bc, true); break; // SLA B
        case 0x21: Logger::print("SLA C\n"); sla_r8(reg_bc, false); break; // SLA C
        case 0x22: Logger::print("SLA D\n"); sla_r8(reg_de, true); break; // SLA D
        case 0x23: Logger::print("SLA E\n"); sla_r8(reg_de, false); break; // SLA E
        case 0x24: Logger::print("SLA H\n"); sla_r8(reg_hl, true); break; // SLA H
        case 0x25: Logger::print("SLA L\n"); sla_r8(reg_hl, false); break; // SLA L
        case 0x26: Logger::print("SLA [HL]\n"); sla_mHL(); break; // SLA [HL]
        case 0x27: Logger::print("SLA A\n"); sla_r8(reg_af, true); break; // SA A
        case 0x28: Logger::print("SRA B\n"); sra_r8(reg_bc, true); break; // SRA B
        case 0x29: Logger::print("SRA C\n"); sra_r8(reg_bc, false); break; // SRA C
        case 0x2A: Logger::print("SRA D\n"); sra_r8(reg_de, true); break; // SRA D
        case 0x2B: Logger::print("SRA E\n"); sra_r8(reg_de, false); break; // SRA E
        case 0x2C: Logger::print("SRA H\n"); sra_r8(reg_hl, true); break; // SRA H
        case 0x2D: Logger::print("SRA L\n"); sra_r8(reg_hl, false); break; // SRA L
        case 0x2E: Logger::print("SRA [HL]\n"); sra_mHL(); break; // SRA [HL]
        case 0x2F: Logger::print("SRA A\n"); sra_r8(reg_af, true); break; // SRA A

        case 0x30: Logger::print("SWAP B\n"); swap_r8(reg_bc, true); break; // SWAP B
        case 0x31: Logger::print("SWAP C\n"); swap_r8(reg_bc, false); break; // SWAP C
        case 0x32: Logger::print("SWAP D\n"); swap_r8(reg_de, true); break; // SWAP D
        case 0x33: Logger::print("SWAP E\n"); swap_r8(reg_de, false); break; // SWAP E
        case 0x34: Logger::print("SWAP H\n"); swap_r8(reg_hl, true); break; // SWAP H
        case 0x35: Logger::print("SWAP L\n"); swap_r8(reg_hl, false); break; // SWAP L
        case 0x36: Logger::print("SWAP [HL]\n"); swap_mHL(); break; // SWAP [HL]
        case 0x37: Logger::print("SWAP A\n"); swap_r8(reg_af, true); break; // SWAP A
        case 0x38: Logger::print("SRL B\n"); srl_r8(reg_bc, true); break; // SRL B
        case 0x39: Logger::print("SRL C\n"); srl_r8(reg_bc, false); break; // SRL C
        case 0x3A: Logger::print("SRL D\n"); srl_r8(reg_de, true); break; // SRL D
        case 0x3B: Logger::print("SRL E\n"); srl_r8(reg_de, false); break; // SRL E
        case 0x3C: Logger::print("SRL H\n"); srl_r8(reg_hl, true); break; // SRL H
        case 0x3D: Logger::print("SRL L\n"); srl_r8(reg_hl, false); break; // SRL L
        case 0x3E: Logger::print("SRL [HL]\n"); srl_mHL(); break; // SRL [HL]
        case 0x3F: Logger::print("SRL A\n"); srl_r8(reg_af, true); break; // SRL A

        case 0x40: Logger::print("BIT 0, B\n"); bit_r8(reg_bc, true, 0); break; // BIT 0, B
        case 0x41: Logger::print("BIT 0, C\n"); bit_r8(reg_bc, false, 0); break; // BIT 0, C
        case 0x42: Logger::print("BIT 0, D\n"); bit_r8(reg_de, true, 0); break; // BIT 0, D
        case 0x43: Logger::print("BIT 0, E\n"); bit_r8(reg_de, false, 0); break; // BIT 0, E
        case 0x44: Logger::print("BIT 0, H\n"); bit_r8(reg_hl, true, 0); break; // BIT 0, H
        case 0x45: Logger::print("BIT 0, L\n"); bit_r8(reg_hl, false, 0); break; // BIT 0, L
        case 0x46: Logger::print("BIT 0, [HL]\n"); bit_mHL(0); break; // BIT 0, [HL]
        case 0x47: Logger::print("BIT 0, A\n"); bit_r8(reg_af, true, 0); break; // BIT 0, A
        case 0x48: Logger::print("BIT 1, B\n"); bit_r8(reg_bc, true, 1); break; // BIT 1, B
        case 0x49: Logger::print("BIT 1, C\n"); bit_r8(reg_bc, false, 1); break; // BIT 1, C
        case 0x4A: Logger::print("BIT 1, D\n"); bit_r8(reg_de, true, 1); break; // BIT 1, D
        case 0x4B: Logger::print("BIT 1, E\n"); bit_r8(reg_de, false, 1); break; // BIT 1, E
        case 0x4C: Logger::print("BIT 1, H\n"); bit_r8(reg_hl, true, 1); break; // BIT 1, H
        case 0x4D: Logger::print("BIT 1, L\n"); bit_r8(reg_hl, false, 1); break; // BIT 1, L
        case 0x4E: Logger::print("BIT 1, [HL]\n"); bit_mHL(1); break; // BIT 1, [HL]
        case 0x4F: Logger::print("BIT 1, A\n"); bit_r8(reg_af, true, 1); break; // BIT 1, A

        case 0x50: Logger::print("BIT 2, B\n"); bit_r8(reg_bc, true, 2); break; // BIT 2, B
        case 0x51: Logger::print("BIT 2, C\n"); bit_r8(reg_bc, false, 2); break; // BIT 2, C
        case 0x52: Logger::print("BIT 2, D\n"); bit_r8(reg_de, true, 2); break; // BIT 2, D
        case 0x53: Logger::print("BIT 2, E\n"); bit_r8(reg_de, false, 2); break; // BIT 2, E
        case 0x54: Logger::print("BIT 2, H\n"); bit_r8(reg_hl, true, 2); break; // BIT 2, H
        case 0x55: Logger::print("BIT 2, L\n"); bit_r8(reg_hl, false, 2); break; // BIT 2, L
        case 0x56: Logger::print("BIT 2, [HL]\n"); bit_mHL(2); break; // BIT 2, [HL]
        case 0x57: Logger::print("BIT 2, A\n"); bit_r8(reg_af, true, 2); break; // BIT 2, A
        case 0x58: Logger::print("BIT 3, B\n"); bit_r8(reg_bc, true, 3); break; // BIT 3, B
        case 0x59: Logger::print("BIT 3, C\n"); bit_r8(reg_bc, false, 3); break; // BIT 3, C
        case 0x5A: Logger::print("BIT 3, D\n"); bit_r8(reg_de, true, 3); break; // BIT 3, D
        case 0x5B: Logger::print("BIT 3, E\n"); bit_r8(reg_de, false, 3); break; // BIT 3, E
        case 0x5C: Logger::print("BIT 3, H\n"); bit_r8(reg_hl, true, 3); break; // BIT 3, H
        case 0x5D: Logger::print("BIT 3, L\n"); bit_r8(reg_hl, false, 3); break; // BIT 3, L
        case 0x5E: Logger::print("BIT 3, [HL]\n"); bit_mHL(3); break; // BIT 3, [HL]
        case 0x5F: Logger::print("BIT 3, A\n"); bit_r8(reg_af, true, 3); break; // BIT 3, A

        case 0x60: Logger::print("BIT 4, B\n"); bit_r8(reg_bc, true, 4); break; // BIT 4, B
        case 0x61: Logger::print("BIT 4, C\n"); bit_r8(reg_bc, false, 4); break; // BIT 4, C
        case 0x62: Logger::print("BIT 4, D\n"); bit_r8(reg_de, true, 4); break; // BIT 4, D
        case 0x63: Logger::print("BIT 4, E\n"); bit_r8(reg_de, false, 4); break; // BIT 4, E
        case 0x64: Logger::print("BIT 4, H\n"); bit_r8(reg_hl, true, 4); break; // BIT 4, H
        case 0x65: Logger::print("BIT 4, L\n"); bit_r8(reg_hl, false, 4); break; // BIT 4, L
        case 0x66: Logger::print("BIT 4, [HL]\n"); bit_mHL(4); break; // BIT 4, [HL]
        case 0x67: Logger::print("BIT 4, A\n"); bit_r8(reg_af, true, 4); break; // BIT 4, A
        case 0x68: Logger::print("BIT 5, B\n"); bit_r8(reg_bc, true, 5); break; // BIT 5, B
        case 0x69: Logger::print("BIT 5, C\n"); bit_r8(reg_bc, false, 5); break; // BIT 5, C
        case 0x6A: Logger::print("BIT 5, D\n"); bit_r8(reg_de, true, 5); break; // BIT 5, D
        case 0x6B: Logger::print("BIT 5, E\n"); bit_r8(reg_de, false, 5); break; // BIT 5, E
        case 0x6C: Logger::print("BIT 5, H\n"); bit_r8(reg_hl, true, 5); break; // BIT 5, H
        case 0x6D: Logger::print("BIT 5, L\n"); bit_r8(reg_hl, false, 5); break; // BIT 5, L
        case 0x6E: Logger::print("BIT 5, [HL]\n"); bit_mHL(5); break; // BIT 5, [HL]
        case 0x6F: Logger::print("BIT 5, A\n"); bit_r8(reg_af, true, 5); break; // BIT 5, A

        case 0x70: Logger::print("BIT 6, B\n"); bit_r8(reg_bc, true, 6); break; // BIT 6, B
        case 0x71: Logger::print("BIT 6, C\n"); bit_r8(reg_bc, false, 6); break; // BIT 6, C
        case 0x72: Logger::print("BIT 6, D\n"); bit_r8(reg_de, true, 6); break; // BIT 6, D
        case 0x73: Logger::print("BIT 6, E\n"); bit_r8(reg_de, false, 6); break; // BIT 6, E
        case 0x74: Logger::print("BIT 6, H\n"); bit_r8(reg_hl, true, 6); break; // BIT 6, H
        case 0x75: Logger::print("BIT 6, L\n"); bit_r8(reg_hl, false, 6); break; // BIT 6, L
        case 0x76: Logger::print("BIT 6, [HL]\n"); bit_mHL(6); break; // BIT 6, [HL]
        case 0x77: Logger::print("BIT 6, A\n"); bit_r8(reg_af, true, 6); break; // BIT 6, A
        case 0x78: Logger::print("BIT 7, B\n"); bit_r8(reg_bc, true, 7); break; // BIT 7, B
        case 0x79: Logger::print("BIT 7, C\n"); bit_r8(reg_bc, false, 7); break; // BIT 7, C
        case 0x7A: Logger::print("BIT 7, D\n"); bit_r8(reg_de, true, 7); break; // BIT 7, D
        case 0x7B: Logger::print("BIT 7, E\n"); bit_r8(reg_de, false, 7); break; // BIT 7, E
        case 0x7C: Logger::print("BIT 7, H\n"); bit_r8(reg_hl, true, 7); break; // BIT 7, H
        case 0x7D: Logger::print("BIT 7, L\n"); bit_r8(reg_hl, false, 7); break; // BIT 7, L
        case 0x7E: Logger::print("BIT 7, [HL]\n"); bit_mHL(7); break; // BIT 7, [HL]
        case 0x7F: Logger::print("BIT 7, A\n"); bit_r8(reg_af, true, 7); break; // BIT 7, A

        case 0x80: Logger::print("RES 0, B\n"); res_r8(reg_bc, true, 0); break; // RES 0, B
        case 0x81: Logger::print("RES 0, C\n"); res_r8(reg_bc, false, 0); break; // RES 0, C
        case 0x82: Logger::print("RES 0, D\n"); res_r8(reg_de, true, 0); break; // RES 0, D
        case 0x83: Logger::print("RES 0, E\n"); res_r8(reg_de, false, 0); break; // RES 0, E
        case 0x84: Logger::print("RES 0, H\n"); res_r8(reg_hl, true, 0); break; // RES 0, H
        case 0x85: Logger::print("RES 0, L\n"); res_r8(reg_hl, false, 0); break; // RES 0, L
        case 0x86: Logger::print("RES 0, [HL]\n"); res_mHL(0); break; // RES 0, [HL]
        case 0x87: Logger::print("RES 0, A\n"); res_r8(reg_af, true, 0); break; // RES 0, A
        case 0x88: Logger::print("RES 1, B\n"); res_r8(reg_bc, true, 1); break; // RES 1, B
        case 0x89: Logger::print("RES 1, C\n"); res_r8(reg_bc, false, 1); break; // RES 1, C
        case 0x8A: Logger::print("RES 1, D\n"); res_r8(reg_de, true, 1); break; // RES 1, D
        case 0x8B: Logger::print("RES 1, E\n"); res_r8(reg_de, false, 1); break; // RES 1, E
        case 0x8C: Logger::print("RES 1, H\n"); res_r8(reg_hl, true, 1); break; // RES 1, H
        case 0x8D: Logger::print("RES 1, L\n"); res_r8(reg_hl, false, 1); break; // RES 1, L
        case 0x8E: Logger::print("RES 1, [HL]\n"); res_mHL(1); break; // RES 1, [HL]
        case 0x8F: Logger::print("RES 1, A\n"); res_r8(reg_af, true, 1); break; // RES 1, A

        case 0x90: Logger::print("RES 2, B\n"); res_r8(reg_bc, true, 2); break; // RES 2, B
        case 0x91: Logger::print("RES 2, C\n"); res_r8(reg_bc, false, 2); break; // RES 2, C
        case 0x92: Logger::print("RES 2, D\n"); res_r8(reg_de, true, 2); break; // RES 2, D
        case 0x93: Logger::print("RES 2, E\n"); res_r8(reg_de, false, 2); break; // RES 2, E
        case 0x94: Logger::print("RES 2, H\n"); res_r8(reg_hl, true, 2); break; // RES 2, H
        case 0x95: Logger::print("RES 2, L\n"); res_r8(reg_hl, false, 2); break; // RES 2, L
        case 0x96: Logger::print("RES 2, [HL]\n"); res_mHL(2); break; // RES 2, [HL]
        case 0x97: Logger::print("RES 2, A\n"); res_r8(reg_af, true, 2); break; // RES 2, A
        case 0x98: Logger::print("RES 3, B\n"); res_r8(reg_bc, true, 3); break; // RES 3, B
        case 0x99: Logger::print("RES 3, C\n"); res_r8(reg_bc, false, 3); break; // RES 3, C
        case 0x9A: Logger::print("RES 3, D\n"); res_r8(reg_de, true, 3); break; // RES 3, D
        case 0x9B: Logger::print("RES 3, E\n"); res_r8(reg_de, false, 3); break; // RES 3, E
        case 0x9C: Logger::print("RES 3, H\n"); res_r8(reg_hl, true, 3); break; // RES 3, H
        case 0x9D: Logger::print("RES 3, L\n"); res_r8(reg_hl, false, 3); break; // RES 3, L
        case 0x9E: Logger::print("RES 3, [HL]\n"); res_mHL(3); break; // RES 3, [HL]
        case 0x9F: Logger::print("RES 3, A\n"); res_r8(reg_af, true, 3); break; // RES 3, A

        case 0xA0: Logger::print("RES 4, B\n"); res_r8(reg_bc, true, 4); break; // RES 4, B
        case 0xA1: Logger::print("RES 4, C\n"); res_r8(reg_bc, false, 4); break; // RES 4, C
        case 0xA2: Logger::print("RES 4, D\n"); res_r8(reg_de, true, 4); break; // RES 4, D
        case 0xA3: Logger::print("RES 4, E\n"); res_r8(reg_de, false, 4); break; // RES 4, E
        case 0xA4: Logger::print("RES 4, H\n"); res_r8(reg_hl, true, 4); break; // RES 4, H
        case 0xA5: Logger::print("RES 4, L\n"); res_r8(reg_hl, false, 4); break; // RES 4, L
        case 0xA6: Logger::print("RES 4, [HL]\n"); res_mHL(4); break; // RES 4, [HL]
        case 0xA7: Logger::print("RES 4, A\n"); res_r8(reg_af, true, 4); break; // RES 4, A
        case 0xA8: Logger::print("RES 5, B\n"); res_r8(reg_bc, true, 5); break; // RES 5, B
        case 0xA9: Logger::print("RES 5, C\n"); res_r8(reg_bc, false, 5); break; // RES 5, C
        case 0xAA: Logger::print("RES 5, D\n"); res_r8(reg_de, true, 5); break; // RES 5, D
        case 0xAB: Logger::print("RES 5, E\n"); res_r8(reg_de, false, 5); break; // RES 5, E
        case 0xAC: Logger::print("RES 5, H\n"); res_r8(reg_hl, true, 5); break; // RES 5, H
        case 0xAD: Logger::print("RES 5, L\n"); res_r8(reg_hl, false, 5); break; // RES 5, L
        case 0xAE: Logger::print("RES 5, [HL]\n"); res_mHL(5); break; // RES 5, [HL]
        case 0xAF: Logger::print("RES 5, A\n"); res_r8(reg_af, true, 5); break; // RES 5, A

        case 0xB0: Logger::print("RES 6, B\n"); res_r8(reg_bc, true, 6); break; // RES 6, B
        case 0xB1: Logger::print("RES 6, C\n"); res_r8(reg_bc, false, 6); break; // RES 6, C
        case 0xB2: Logger::print("RES 6, D\n"); res_r8(reg_de, true, 6); break; // RES 6, D
        case 0xB3: Logger::print("RES 6, E\n"); res_r8(reg_de, false, 6); break; // RES 6, E
        case 0xB4: Logger::print("RES 6, H\n"); res_r8(reg_hl, true, 6); break; // RES 6, H
        case 0xB5: Logger::print("RES 6, L\n"); res_r8(reg_hl, false, 6); break; // RES 6, L
        case 0xB6: Logger::print("RES 6, [HL]\n"); res_mHL(6); break; // RES 6, [HL]
        case 0xB7: Logger::print("RES 6, B\n"); res_r8(reg_af, true, 6); break; // RES 6, B
        case 0xB8: Logger::print("RES 7, B\n"); res_r8(reg_bc, true, 7); break; // RES 7, B
        case 0xB9: Logger::print("RES 7, C\n"); res_r8(reg_bc, false, 7); break; // RES 7, C
        case 0xBA: Logger::print("RES 7, D\n"); res_r8(reg_de, true, 7); break; // RES 7, D
        case 0xBB: Logger::print("RES 7, E\n"); res_r8(reg_de, false, 7); break; // RES 7, E
        case 0xBC: Logger::print("RES 7, H\n"); res_r8(reg_hl, true, 7); break; // RES 7, H
        case 0xBD: Logger::print("RES 7, L\n"); res_r8(reg_hl, false, 7); break; // RES 7, L
        case 0xBE: Logger::print("RES 7, [HL]\n"); res_mHL(7); break; // RES 7, [HL]
        case 0xBF: Logger::print("RES 7, B\n"); res_r8(reg_af, true, 7); break; // RES 7, B

        case 0xC0: Logger::print("SET 0, B\n"); set_r8(reg_bc, true, 0); break; // SET 0, B
        case 0xC1: Logger::print("SET 0, C\n"); set_r8(reg_bc, false, 0); break; // SET 0, C
        case 0xC2: Logger::print("SET 0, D\n"); set_r8(reg_de, true, 0); break; // SET 0, D
        case 0xC3: Logger::print("SET 0, E\n"); set_r8(reg_de, false, 0); break; // SET 0, E
        case 0xC4: Logger::print("SET 0, H\n"); set_r8(reg_hl, true, 0); break; // SET 0, H
        case 0xC5: Logger::print("SET 0, L\n"); set_r8(reg_hl, false, 0); break; // SET 0, L
        case 0xC6: Logger::print("SET 0, [HL]\n"); set_mHL(0); break; // SET 0, [HL]
        case 0xC7: Logger::print("SET 0, A\n"); set_r8(reg_af, true, 0); break; // SET 0, A
        case 0xC8: Logger::print("SET 1, B\n"); set_r8(reg_bc, true, 1); break; // SET 1, B
        case 0xC9: Logger::print("SET 1, C\n"); set_r8(reg_bc, false, 1); break; // SET 1, C
        case 0xCA: Logger::print("SET 1, D\n"); set_r8(reg_de, true, 1); break; // SET 1, D
        case 0xCB: Logger::print("SET 1, E\n"); set_r8(reg_de, false, 1); break; // SET 1, E
        case 0xCC: Logger::print("SET 1, H\n"); set_r8(reg_hl, true, 1); break; // SET 1, H
        case 0xCD: Logger::print("SET 1, L\n"); set_r8(reg_hl, false, 1); break; // SET 1, L
        case 0xCE: Logger::print("SET 1, [HL]\n"); set_mHL(1); break; // SET 1, [HL]
        case 0xCF: Logger::print("SET 1, A\n"); set_r8(reg_af, true, 1); break; // SET 1, A

        case 0xD0: Logger::print("SET 2, B\n"); set_r8(reg_bc, true, 2); break; // SET 2, B
        case 0xD1: Logger::print("SET 2, C\n"); set_r8(reg_bc, false, 2); break; // SET 2, C
        case 0xD2: Logger::print("SET 2, D\n"); set_r8(reg_de, true, 2); break; // SET 2, D
        case 0xD3: Logger::print("SET 2, E\n"); set_r8(reg_de, false, 2); break; // SET 2, E
        case 0xD4: Logger::print("SET 2, H\n"); set_r8(reg_hl, true, 2); break; // SET 2, H
        case 0xD5: Logger::print("SET 2, L\n"); set_r8(reg_hl, false, 2); break; // SET 2, L
        case 0xD6: Logger::print("SET 2, [HL]\n"); set_mHL(2); break; // SET 2, [HL]
        case 0xD7: Logger::print("SET 2, A\n"); set_r8(reg_af, true, 2); break; // SET 2, A
        case 0xD8: Logger::print("SET 3, B\n"); set_r8(reg_bc, true, 3); break; // SET 3, B
        case 0xD9: Logger::print("SET 3, C\n"); set_r8(reg_bc, false, 3); break; // SET 3, C
        case 0xDA: Logger::print("SET 3, D\n"); set_r8(reg_de, true, 3); break; // SET 3, D
        case 0xDB: Logger::print("SET 3, E\n"); set_r8(reg_de, false, 3); break; // SET 3, E
        case 0xDC: Logger::print("SET 3, H\n"); set_r8(reg_hl, true, 3); break; // SET 3, H
        case 0xDD: Logger::print("SET 3, L\n"); set_r8(reg_hl, false, 3); break; // SET 3, L
        case 0xDE: Logger::print("SET 3, [HL]\n"); set_mHL(3); break; // SET 3, [HL]
        case 0xDF: Logger::print("SET 3, A\n"); set_r8(reg_af, true, 3); break; // SET 3, A

        case 0xE0: Logger::print("SET 4, B\n"); set_r8(reg_bc, true, 4); break; // SET 4, B
        case 0xE1: Logger::print("SET 4, C\n"); set_r8(reg_bc, false, 4); break; // SET 4, C
        case 0xE2: Logger::print("SET 4, D\n"); set_r8(reg_de, true, 4); break; // SET 4, D
        case 0xE3: Logger::print("SET 4, E\n"); set_r8(reg_de, false, 4); break; // SET 4, E
        case 0xE4: Logger::print("SET 4, H\n"); set_r8(reg_hl, true, 4); break; // SET 4, H
        case 0xE5: Logger::print("SET 4, L\n"); set_r8(reg_hl, false, 4); break; // SET 4, L
        case 0xE6: Logger::print("SET 4, [HL]\n"); set_mHL(4); break; // SET 4, [HL]
        case 0xE7: Logger::print("SET 4, A\n"); set_r8(reg_af, true, 4); break; // SET 4, A
        case 0xE8: Logger::print("SET 5, B\n"); set_r8(reg_bc, true, 5); break; // SET 5, B
        case 0xE9: Logger::print("SET 5, C\n"); set_r8(reg_bc, false, 5); break; // SET 5, C
        case 0xEA: Logger::print("SET 5, D\n"); set_r8(reg_de, true, 5); break; // SET 5, D
        case 0xEB: Logger::print("SET 5, E\n"); set_r8(reg_de, false, 5); break; // SET 5, E
        case 0xEC: Logger::print("SET 5, H\n"); set_r8(reg_hl, true, 5); break; // SET 5, H
        case 0xED: Logger::print("SET 5, L\n"); set_r8(reg_hl, false, 5); break; // SET 5, L
        case 0xEE: Logger::print("SET 5, [HL]\n"); set_mHL(5); break; // SET 5, [HL]
        case 0xEF: Logger::print("SET 5, A\n"); set_r8(reg_af, true, 5); break; // SET 5, A

        case 0xF0: Logger::print("SET 6, B\n"); set_r8(reg_bc, true, 6); break; // SET 6, B
        case 0xF1: Logger::print("SET 6, C\n"); set_r8(reg_bc, false, 6); break; // SET 6, C
        case 0xF2: Logger::print("SET 6, D\n"); set_r8(reg_de, true, 6); break; // SET 6, D
        case 0xF3: Logger::print("SET 6, E\n"); set_r8(reg_de, false, 6); break; // SET 6, E
        case 0xF4: Logger::print("SET 6, H\n"); set_r8(reg_hl, true, 6); break; // SET 6, H
        case 0xF5: Logger::print("SET 6, L\n"); set_r8(reg_hl, false, 6); break; // SET 6, L
        case 0xF6: Logger::print("SET 6, [HL]\n"); set_mHL(6); break; // SET 6, [HL]
        case 0xF7: Logger::print("SET 6, B\n"); set_r8(reg_af, true, 6); break; // SET 6, B
        case 0xF8: Logger::print("SET 7, B\n"); set_r8(reg_bc, true, 7); break; // SET 7, B
        case 0xF9: Logger::print("SET 7, C\n"); set_r8(reg_bc, false, 7); break; // SET 7, C
        case 0xFA: Logger::print("SET 7, D\n"); set_r8(reg_de, true, 7); break; // SET 7, D
        case 0xFB: Logger::print("SET 7, E\n"); set_r8(reg_de, false, 7); break; // SET 7, E
        case 0xFC: Logger::print("SET 7, H\n"); set_r8(reg_hl, true, 7); break; // SET 7, H
        case 0xFD: Logger::print("SET 7, L\n"); set_r8(reg_hl, false, 7); break; // SET 7, L
        case 0xFE: Logger::print("SET 7, [HL]\n"); set_mHL(7); break; // SET 7, [HL]
        case 0xFF: Logger::print("SET 7, B\n"); set_r8(reg_af, true, 7); break; // SET 7, B

        default: fmt::print("UNKNOWN OPCODE\n");
    }
}


//instructions
void CPU::ld_r8_u8(uint16_t &reg, bool is_high) // LD r8, u8
{
    uint8_t val = fetch_byte();
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);
}
void CPU::ld_r8_r8(uint16_t &reg1, bool is_high_reg1, uint16_t &reg2, bool is_high_reg2)
{
    uint8_t val {};
    if(is_high_reg2)
        val = Register::get_register_high(reg2);
    else 
        val = Register::get_register_low(reg2);
    
    if(is_high_reg1)
        Register::set_register_high(reg1, val);
    else 
        Register::set_register_low(reg1, val);
}
void CPU::ld_r8_mHL(uint16_t &reg, bool is_high) // LD r8, [HL]
{
    uint8_t val = read(reg_hl);
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);
}
void CPU::ld_mHL_r8(uint16_t &reg, bool is_high) // LD [HL], r8
{
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    write(reg_hl, val);    
}
void CPU::ld_mHL_u8() // LD [HL], u8
{
    increment_cycle();
    uint8_t val = fetch_byte();
    write(reg_hl, val);
}
void CPU::ld_mHLi_A() // LD [HL+], A
{
    write(reg_hl, get_reg_A());
    reg_hl++;
}
void CPU::ld_A_mHLi() // LD A, [HL+]
{
    uint8_t val = read(reg_hl);
    reg_hl++;
    set_reg_A(val);
}
void CPU::ld_mHLd_A() // LD [HL-], A
{
    write(reg_hl, get_reg_A());
    reg_hl--;
}
void CPU::ld_A_mHLd() // LD A, [HL-]
{
    uint8_t val = read(reg_hl);
    reg_hl--;
    set_reg_A(val);
}
void CPU::ld_ffu8_A() // LD [FF00+u8], A
{
    increment_cycle();
    uint8_t val = fetch_byte();
    write(0xFF00 + val, get_reg_A());
}
void CPU::ld_ffC_A() // LD [FF00+C], A
{
    write(0xFF00 + Register::get_register_low(reg_bc), get_reg_A());
}
void CPU::ld_A_ffu8() // LD A, [FF00+u8]
{
    uint8_t u8 = fetch_byte();
    uint8_t val = read(0xFF00 + u8);
    set_reg_A(val);
}
void CPU::ld_A_ffC() // LD A, [FF00+C]
{
    uint8_t val = read(0xFF00 + Register::get_register_low(reg_bc));
    set_reg_A(val);
}

void CPU::ld_r16_u16(uint16_t &reg) // LD r16, u16
{
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t val = (high << 8) | low;
    reg = val;
}
void CPU::ld_A_mr16(uint16_t &reg) // LD A, [r16]
{
    uint8_t val = read(reg);
    set_reg_A(val);
}
void CPU::ld_mr16_A(uint16_t &reg) // LD [r16], A
{
    write(reg, get_reg_A());
}
void CPU::ld_mu16_A() // LD [u16], A
{
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t addr = (high << 8) | low;
    write(addr, get_reg_A());
}
void CPU::ld_A_mu16() // LD A, [u16]
{
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t addr = (high << 8) | low;
    uint8_t val = read(addr);
    set_reg_A(val);
}
void CPU::ld_SP_u16() // LD SP, u16
{
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t val = (high << 8) | low;
    reg_sp = val;
}
void CPU::ld_HL_SPi8() // LD HL, SP+i8
{
    increment_cycle();
    int8_t val = fetch_byte();
    reg_hl += reg_sp + val;
}
void CPU::ld_SP_HL() // LD SP, HL
{
    increment_cycle();
    reg_sp = reg_hl;
}

void CPU::inc_r8(uint16_t &reg, bool is_high) // INC r8
{
    uint8_t val, temp {};
    if(is_high)
    {
        val = Register::get_register_high(reg);
        temp = val;
        val++;
        Register::set_register_high(reg, val);
    }
    else 
    {
        val = Register::get_register_low(reg);
        temp = val;
        val++;
        Register::set_register_low(reg, val);
    }  
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag((temp & 0x0F) == 0x0F); //Set if overflow from bit 3
}
void CPU::inc_mHL() // INC [HL]
{
    uint8_t val = read(reg_hl);
    uint8_t temp = val;
    val++;
    write(reg_hl, val);
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag((temp & 0x0F) == 0x0F); //Set if overflow from bit 3
}
void CPU::dec_r8(uint16_t &reg, bool is_high) // DEC r8
{
    uint8_t val, temp {};
    if(is_high)
    {
        val = Register::get_register_high(reg);
        temp = val;
        val--;
        Register::set_register_high(reg, val);
    }
    else 
    {
        val = Register::get_register_low(reg);
        temp = val;
        val--;
        Register::set_register_low(reg, val);
    }
    set_zero_flag(val == 0);
    set_subtraction_flag(false); 
    set_halfcarry_flag((temp & 0x0F) == 0); //Set if borrow from bit 4
}
void CPU::dec_mHL() // DEC [HL]
{
    uint8_t val = read(reg_hl);
    uint8_t temp = val;
    val--;
    write(reg_hl, val);
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag((temp & 0x0F) == 0); //Set if borrow from bit 4
} 

void CPU::add_A_r8(uint16_t reg, bool is_high, bool carry) // ADD A, r8
{
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else
        val = Register::get_register_low(reg);
    uint8_t a = get_reg_A();
    a += val + carry;

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag((get_reg_A() & 0b1111) + (val & 0b1111) + carry > 0b1111);
    set_carry_flag((get_reg_A() & 0b1111'1111) + (val & 0b1111'1111) + carry > 0b1111'1111);
    set_reg_A(a);
}
void CPU::add_A_mHL(bool carry) // ADD A, [HL]
{
    uint8_t val = read(reg_hl);
    uint8_t a = get_reg_A();
    a += val + carry;

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag((get_reg_A() & 0b1111) + (val & 0b1111) + carry > 0b1111);
    set_carry_flag((get_reg_A() & 0b1111'1111) + (val & 0b1111'1111) + carry > 0b1111'1111);
    set_reg_A(a);
}
void CPU::add_A_u8(bool carry) // ADD A, u8
{
    uint8_t val = fetch_byte();
    uint8_t a = get_reg_A();
    a += val + carry;

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag((get_reg_A() & 0b1111) + (val & 0b1111) + carry > 0b1111);
    set_carry_flag((get_reg_A() & 0b1111'1111) + (val & 0b1111'1111) + carry > 0b1111'1111);
    set_reg_A(a); 
}
void CPU::sub_A_r8(uint16_t reg, bool is_high, bool carry) // SUB A, r8
{
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else
        val = Register::get_register_low(reg);
    uint8_t a = get_reg_A();
    a -= (val + carry);

    set_zero_flag(a == 0);
    set_subtraction_flag(true);
    set_halfcarry_flag((get_reg_A() & 0b1111) - ((val & 0b1111) + carry) < 0);
    set_carry_flag((val + carry) > get_reg_A());
    set_reg_A(a);
}
void CPU::sub_A_mHL(bool carry) // SUB A, [HL]
{
    uint8_t val = read(reg_hl);
    uint8_t a = get_reg_A();
    a -= (val + carry);

    set_zero_flag(a == 0);
    set_subtraction_flag(true);
    set_halfcarry_flag((get_reg_A() & 0b1111) - ((val & 0b1111) + carry) < 0);
    set_carry_flag((val + carry) > get_reg_A());
    set_reg_A(a);
}
void CPU::sub_A_u8(bool carry) // SUB A, u8
{
    uint8_t val = fetch_byte();
    uint8_t a = get_reg_A();
    a -= (val + carry);

    set_zero_flag(a == 0);
    set_subtraction_flag(true);
    set_halfcarry_flag((get_reg_A() & 0b1111) - ((val & 0b1111) + carry) < 0);
    set_carry_flag((val + carry) > get_reg_A());
    set_reg_A(a);
}


void CPU::and_A_r8(uint16_t reg, bool is_high) // AND A, r8
{
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    uint8_t a = get_reg_A();
    a &= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(true);
    set_carry_flag(false);
}
void CPU::and_A_mHL() // AND A, [HL]
{
    uint8_t val = read(reg_hl);
    uint8_t a = get_reg_A();
    a &= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(true);
    set_carry_flag(false);
}
void CPU::and_A_u8() // AND A, u8
{
    uint8_t val = fetch_byte();
    uint8_t a = get_reg_A();
    a &= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(true);
    set_carry_flag(false);
}

void CPU::xor_A_r8(uint16_t reg, bool is_high) // XOR A, r8
{
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    uint8_t a = get_reg_A();
    a ^= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(false);
}
void CPU::xor_A_mHL() // XOR A, [HL]
{
    uint8_t val = read(reg_hl);
    uint8_t a = get_reg_A();
    a ^= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(false);
}
void CPU::xor_A_u8() // XOR A, u8
{
    uint8_t val = fetch_byte();
    uint8_t a = get_reg_A();
    a ^= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(false);
}
void CPU::or_A_r8(uint16_t reg, bool is_high) // OR A, r8
{
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else
        val = Register::get_register_low(reg);
    uint8_t a = get_reg_A();
    a |= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(false);
}
void CPU::or_A_mHL() // OR A, [HL]
{
    uint8_t val = read(reg_hl);
    uint8_t a = get_reg_A();
    a |= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(false);
}
void CPU::or_A_u8() // OR A, u8
{
    uint8_t val = fetch_byte();
    uint8_t a = get_reg_A();
    a |= val;
    set_reg_A(a);

    set_zero_flag(a == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(false);
}
void CPU::cp_A_r8(uint16_t reg, bool is_high) // CP A, r8
{
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    uint8_t a = get_reg_A();
    a -= val;

    set_zero_flag(a == 0);
    set_subtraction_flag(true);
    set_halfcarry_flag((get_reg_A() & 0b1111) - (val & 0b1111) < 0);
    set_carry_flag(val > get_reg_A());
}
void CPU::cp_A_mHL() // CP A, [HL]
{
    uint8_t val = read(reg_hl);
    uint8_t a = get_reg_A();
    a -= val;

    set_zero_flag(a == 0);
    set_subtraction_flag(true);
    set_halfcarry_flag((get_reg_A() & 0b1111) - (val & 0b1111) < 0);
    set_carry_flag(val > get_reg_A());
}
void CPU::cp_A_u8() // CP A, u8
{
    uint8_t val = fetch_byte();
    uint8_t a = get_reg_A();
    a -= val;

    set_zero_flag(a == 0);
    set_subtraction_flag(true);
    set_halfcarry_flag((get_reg_A() & 0b1111) - (val & 0b1111) < 0);
    set_carry_flag(val > get_reg_A());
}

void CPU::add_HL_r16(uint16_t reg) // ADD HL, r16
{
    increment_cycle();
    uint16_t val = reg_hl + reg;
    set_subtraction_flag(false);
    set_halfcarry_flag((reg_hl & 0b1111'1111'1111) + (reg & 0b1111'1111'1111) > 0b1111'1111'1111); // Set if overflow from bit 11
    set_carry_flag((reg_hl & 0b1111'1111'1111'1111) + (reg & 0b1111'1111'1111'1111) > 0b1111'1111'1111'1111); // Set if overflow from bit 15
    reg_hl = val;
}
void CPU::add_SP_i8() // ADD SP, i8
{
    increment_cycle(2);
    int8_t val = fetch_byte();
    reg_sp += val;
}


void CPU::inc_r16(uint16_t &reg) // INC r16
{
    increment_cycle();
    reg++;
}
void CPU::dec_r16(uint16_t &reg) // DEC r16
{
    increment_cycle();
    reg--;
}

void CPU::ld_mu16_sp() // LD [u16], SP
{
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t addr = (high << 8) | low;
    memory.write(addr, reg_sp & 0x00FF); // sp.low
    memory.write(addr + 1, reg_sp >> 8); // sp.high
}

void CPU::pop(uint16_t &reg) // POP r16
{
    uint8_t low = read(reg_sp);
    reg_sp++;
    uint8_t high = read(reg_sp);
    reg_sp++;
    uint16_t val = (high << 8) | low;
    reg = val;
}
void CPU::push(uint16_t &reg) // PUSH r16
{
    increment_cycle();
    uint8_t high = Register::get_register_high(reg);
    uint8_t low = Register::get_register_low(reg);
    write(--reg_sp, high);
    write(--reg_sp, low);
}

void CPU::ret() // RET
{
    increment_cycle();
    uint8_t low = read(reg_sp);
    reg_sp++;
    uint8_t high = read(reg_sp);
    reg_sp++;
    uint16_t address = (high << 8) | low;
    reg_pc = address;
}
void CPU::ret_cc(bool cc) // RET cc
{
    uint8_t low = read(reg_sp);
    reg_sp++;
    uint8_t high = read(reg_sp);
    reg_sp++;
    uint16_t address = (high << 8) | low;
    if(cc)
    {
        increment_cycle(2);
        reg_pc = address;
    }
    else 
        cycles -= 4; // cycle fix
}
void CPU::reti()
{
    increment_cycle();
    uint8_t low = read(reg_sp);
    reg_sp++;
    uint8_t high = read(reg_sp);
    reg_sp++;
    uint16_t address = (high << 8) | low;
    reg_pc = address;

    //TODO
    interrupt_enabled = true;
}

void CPU::call()
{
    increment_cycle();
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t address = (high << 8) | low;  
    reg_sp--;
    write(reg_sp, Register::get_register_high(reg_pc));
    reg_sp--;
    write(reg_sp, Register::get_register_low(reg_pc));
    reg_pc = address;
}
void CPU::call_cc(bool cc) // CALL cc, u16
{
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t address = (high << 8) | low;

    if(cc)
    {
        increment_cycle();
        reg_sp--;
        write(reg_sp, Register::get_register_high(reg_pc));
        reg_sp--;
        write(reg_sp, Register::get_register_low(reg_pc));
        reg_pc = address;
    }
}
void CPU::rst(uint8_t vec) // RST vec
{
    increment_cycle();
    reg_sp--;
    write(reg_sp, Register::get_register_high(reg_pc));
    reg_sp--;
    write(reg_sp, Register::get_register_low(reg_pc));
    reg_pc = vec;
}


// http://z80-heaven.wikidot.com/instructions-set:daa
// https://ehaskins.com/2018-01-30%20Z80%20DAA/
void CPU::daa() // DAA
{
    uint8_t correction {};
    if(get_halfcarry_flag() || (!get_subtraction_flag() && (get_reg_A() & 0b1111) > 0b1001 ) ) 
    {
        correction |= 0x06;
    }
    if(get_carry_flag() || (!get_subtraction_flag() && ((get_reg_A() >> 4) & 0b1111) > 0b1001 ) )
    {
        correction |= 0x60;
        set_carry_flag(true);
    }
    uint8_t a = get_reg_A();
    a += get_subtraction_flag() ? -correction : correction;
    set_reg_A(a);
    set_carry_flag(a == 0);
    set_halfcarry_flag(false); 
}
void CPU::cpl()
{
    uint8_t a = get_reg_A();
    a = ~a;
    set_reg_A(a);
    set_subtraction_flag(true);
    set_halfcarry_flag(true);
}
void CPU::scf()
{
    set_carry_flag(true);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
}
void CPU::ccf()
{
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    BitOperation::toggle_bit(reg_af, 4); // invert carry flag
}

void CPU::rlca()
{
    uint8_t a = get_reg_A();
    bool carry = a & (1 << 7);
    a = a << 1 | static_cast<uint8_t>(carry);
    set_reg_A(a);

    set_zero_flag(false);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::rrca()
{
    uint8_t a = get_reg_A();
    bool carry = a & 0x01;
    a = a >> 1 | (static_cast<uint8_t>(carry) << 7);
    set_reg_A(a);

    set_zero_flag(false);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::rla()
{
    uint8_t a = get_reg_A();
    bool old_carry = get_carry_flag();
    bool carry = a & (1 << 7);
    a = a << 1 | static_cast<uint8_t>(old_carry);
    set_reg_A(a);

    set_zero_flag(false);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::rra()
{
    uint8_t a = get_reg_A();
    bool old_carry = get_carry_flag();
    bool carry = a & 0x01;
    a = a >> 1 | (static_cast<uint8_t>(old_carry) << 7);
    set_reg_A(a);

    set_zero_flag(false);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
// http://z80-heaven.wikidot.com/instructions-set:rlc
void CPU::rlc_r8(uint16_t &reg, bool is_high) // RLC r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    bool carry = val & (1 << 7);
    val = val << 1 | static_cast<uint8_t>(carry);
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::rlc_mHL() // RLC [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    bool carry = val & (1 << 7);
    val = val << 1 | static_cast<uint8_t>(carry);
    write(reg_hl, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::rrc_r8(uint16_t &reg, bool is_high) // RRC r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    bool carry = val & 0x01;
    val = val >> 1 | (static_cast<uint8_t>(carry) << 7);
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::rrc_mHL() // RRC [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    bool carry = val & 0x01;
    val = val >> 1 | (static_cast<uint8_t>(carry) << 7);
    write(reg_hl, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);   
}
void CPU::rl_r8(uint16_t &reg, bool is_high) // RL r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    bool carry = get_carry_flag();
    bool new_carry = val & (1 << 7);
    val = val << 1 | static_cast<uint8_t>(carry);
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(new_carry);
}
void CPU::rl_mHL() // RL [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    bool carry = get_carry_flag();
    bool new_carry = val & (1 << 7);
    val = val << 1 | static_cast<uint8_t>(carry);
    write(reg_hl, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(new_carry);
}
void CPU::rr_r8(uint16_t &reg, bool is_high) // RR r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    bool carry = get_carry_flag();
    bool new_carry = val & 0x01;
    val = val >> 1 | (static_cast<uint8_t>(carry) << 7);
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(new_carry);
}
void CPU::rr_mHL() // RR [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    bool carry = get_carry_flag();
    bool new_carry = val & 0x01;
    val = val >> 1 | (static_cast<uint8_t>(carry) << 7);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(new_carry);
}
void CPU::sla_r8(uint16_t &reg, bool is_high) // SLA r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    bool carry = static_cast<bool>(val >> 7);
    val = val << 1;
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::sla_mHL() // SLA [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    bool carry = static_cast<bool>(val >> 7);
    val = val << 1;
    write(reg_hl, val);
    
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::sra_r8(uint16_t &reg, bool is_high) // SRA r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    bool carry = val & 0x01;
    bool msb = val & (1 << 7);
    val = val >> 1 | (static_cast<uint8_t>(msb) << 7);
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::sra_mHL() // SR [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    bool carry = val & 0x01;
    bool msb = val & (1 << 7);
    val = val >> 1 | (static_cast<uint8_t>(msb) << 7);
    write(reg_hl, val);

    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::swap_r8(uint16_t &reg, bool is_high) // SWAP r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    val = (val & 0x0F) << 4 | (val & 0xF0) >> 4;
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(false);
}
void CPU::swap_mHL() // SWAP [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    val = (val & 0x0F) << 4 | (val & 0xF0) >> 4;
    write(reg_hl, val);
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(false);
}
void CPU::srl_r8(uint16_t &reg, bool is_high)// SRL r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    bool carry = val & 0x01;
    val = val >> 1;
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}
void CPU::srl_mHL() // SRL [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    bool carry = val & 0x01;
    val = val >> 1;
    write(reg_hl, val);
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);
    set_carry_flag(carry);
}

void CPU::bit_r8(uint16_t &reg, bool is_high, int bit) // BIT u3, r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    bool test_bit = val & (1 << bit);
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false);    
}
void CPU::bit_mHL(int bit) // BIT u3, [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    bool test_bit = val & (1 << bit);
    set_zero_flag(val == 0);
    set_subtraction_flag(false);
    set_halfcarry_flag(false); 
}
void CPU::res_r8(uint16_t &reg, bool is_high, int bit) // RES u3, r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    val = val & ~(1 << bit);
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);
}
void CPU::res_mHL(int bit) // RES u3, [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    val = val & ~(1 << bit);
    write(reg_hl, val);
}
void CPU::set_r8(uint16_t &reg, bool is_high, int bit) // SET u3, r8
{
    increment_cycle();
    uint8_t val {};
    if(is_high)
        val = Register::get_register_high(reg);
    else 
        val = Register::get_register_low(reg);
    val = val | (1 << bit);
    if(is_high)
        Register::set_register_high(reg, val);
    else 
        Register::set_register_low(reg, val);
}
void CPU::set_mHL(int bit) // SET u3, [HL]
{
    increment_cycle();
    uint8_t val = read(reg_hl);
    val = val | (1 << bit);
    write(reg_hl, val);
}


void CPU::jr_i8() // JR i8
{
    increment_cycle();
    int8_t val = fetch_byte();
    reg_pc += val;
}
void CPU::jr_cc_i8(bool cc) // JR CC, i8
{
    increment_cycle();
    int8_t val = fetch_byte();
    if(cc)
    {
        increment_cycle();
        reg_pc += val;
    }
}
void CPU::jp_cc(bool cc) // JP CC, u16
{
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t address = (high << 8) | low;
    if(cc)
    {
        increment_cycle();
        reg_pc = address;
    }
}
void CPU::jp() // JP u16
{
    increment_cycle();
    uint8_t low = fetch_byte();
    uint8_t high = fetch_byte();
    uint16_t address = (high << 8) | low;
    reg_pc = address;
}
void CPU::jp_HL() // JP HL
{
    reg_pc = reg_hl;
}

void CPU::di()
{
    //TODO
    interrupt_enabled = false;
}
void CPU::ei()
{
    //TODO
    interrupt_enabled = true;
}

void CPU::stop()
{
    //TODO
}
void CPU::halt()
{
    //TODO
}

//register helpers
void CPU::set_reg_A(int val)
{
    Register::set_register_high(reg_af, val);
}
void CPU::set_reg_B(int val)
{
    Register::set_register_high(reg_bc, val);
}
void CPU::set_reg_C(int val)
{
    Register::set_register_low(reg_bc, val);
}
void CPU::set_reg_D(int val)
{
    Register::set_register_high(reg_de, val);
}
void CPU::set_reg_E(int val)
{
    Register::set_register_low(reg_de, val);
}
void CPU::set_reg_H(int val)
{
    Register::set_register_high(reg_hl, val);
}
void CPU::set_reg_L(int val)
{
    Register::set_register_low(reg_hl, val);
}

uint8_t CPU::get_reg_A()
{
    return Register::get_register_high(reg_af);
}

uint8_t CPU::get_reg_B()
{
    return Register::get_register_high(reg_bc);
}
uint8_t CPU::get_reg_C()
{
    return Register::get_register_low(reg_bc);
}
uint8_t CPU::get_reg_D()
{
    return Register::get_register_high(reg_de);
}
uint8_t CPU::get_reg_E()
{
    return Register::get_register_low(reg_de);
}
uint8_t CPU::get_reg_H()
{
    return Register::get_register_high(reg_hl);
}
uint8_t CPU::get_reg_L()
{
    return Register::get_register_low(reg_hl);
}

void CPU::set_carry_flag(bool val)
{
    if(val)
        BitOperation::set_bit(reg_af, 4);
    else 
        BitOperation::clear_bit(reg_af, 4);
}
void CPU::set_halfcarry_flag(bool val)
{
    if(val)
        BitOperation::set_bit(reg_af, 5);
    else 
        BitOperation::clear_bit(reg_af, 5);
}
void CPU::set_subtraction_flag(bool val)
{
    if(val)
        BitOperation::set_bit(reg_af, 6);
    else 
        BitOperation::clear_bit(reg_af, 6);
}
void CPU::set_zero_flag(bool val)
{
    if(val)
        BitOperation::set_bit(reg_af, 7);
    else 
        BitOperation::clear_bit(reg_af, 7);
}
bool CPU::get_carry_flag()
{
    return static_cast<bool>(BitOperation::get_bit(reg_af, 4));
}
bool CPU::get_halfcarry_flag()
{
    return static_cast<bool>(BitOperation::get_bit(reg_af, 5));
}
bool CPU::get_subtraction_flag()
{
    return static_cast<bool>(BitOperation::get_bit(reg_af, 6));
}
bool CPU::get_zero_flag()
{
    return static_cast<bool>(BitOperation::get_bit(reg_af, 7));
} 