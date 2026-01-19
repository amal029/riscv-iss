#pragma once

// Risc-v op codes

// See:
// https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf

// Top-level opcodes
#include <cstddef>
enum OPCODES {
  OP_IMM_1 = 0b0010011,  // math operations
  OP_IMM_2 = 0b0000011,  // load
  OP_IMM_3 = 0b1100111,  // jump and link
  OP_IMM_4 = 0b1110011,  // ecall/ebreak
  OP_R = 0b0110011,      // R-type
  OP_S = 0b0100011,      // S-type
  OP_B = 0b1100011,      // B-type
  OP_ULUI = 0b0110111,   // U-type LUI
  OP_UAUIPC = 0b0010111, // U-type AUIPC
  OP_J = 0b1101111       // J-type
};

// R/I-type opcodes funct3
enum RI_FUNC3 {
  OP_F3_ADD = 0x0,
  OP_F3_SUB = 0x0,
  OP_F3_XOR = 0x4,
  OP_F3_OR = 0x6,
  OP_F3_AND = 0x7,
  OP_F3_SLL = 0x1,
  OP_F3_SRL = 0x5,
  OP_F3_SRA = 0x5,
  OP_F3_SLT = 0x2,
  OP_F3_SLTU = 0x3
};

// R-type multiplication extension for integers
enum R_FUNC3 {
  OP_F3_MUL = 0x0,
  OP_F3_MULH = 0x1,
  OP_F3_MULSU = 0x2,
  OP_F3_MULU = 0x3,
  OP_F3_DIV = 0x4,
  OP_F3_DIVU = 0x5,
  OP_F3_REM = 0x6,
  OP_F3_REMU = 0x7
};

// R-type opcodes funct7
enum R_FUNC7 {
  OP_F7_ADD = 0x00,
  OP_F7_SUB = 0x20,
  OP_F7_XOR = 0x00,
  OP_F7_OR = 0x00,
  OP_F7_AND = 0x00,
  OP_F7_SLL = 0x00,
  OP_F7_SRL = 0x00,
  OP_F7_SRA = 0x20,
  OP_F7_SLT = 0x00,
  OP_F7_SLTU = 0x00,
  OP_F7_MUL_DIV = 0x01 // same for all MULs and DIVs
};

// I-type load
enum ILOAD_FUNC3 {
  OP_F3_B = 0x0,
  OP_F3_H = 0x1,
  OP_F3_W = 0x2,
  OP_F3_BU = 0x4,
  OP_F3_HU = 0x5
};

// S-type store func3
enum S_FUNC3 { OP_S_B = 0x0, OP_S_H = 0x1, OP_S_W = 0x2 };

// B-type func3
enum B_FUNC3 {
  OP_F3_BEQ = 0x0,
  OP_F3_BNE = 0x1,
  OP_F3_BLT = 0x4,
  OP_F3_BGE = 0x5,
  OP_F3_BLTU = 0x6,
  OP_F3_BGEU = 0x7
};

// Jump and link Func3
enum IJ_FUNC3 { OP_F3_JALR = 0x0 };

// Ecall/ebreak F3
enum IE_FUNC3 { OP_F3_ECALL = 0x0, OP_F3_EBREAK = 0x0 };

// Ecall/Ebreak F7
enum IE_FUN7 { OP_F7_ECALL = 0x0, OP_F7_EBREAK = 0x1 };
