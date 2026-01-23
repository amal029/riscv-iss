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
  OP_J = 0b1101111,      // J-type
};

enum Float_OPCODES {
  Float_I = 0b0000111,
  Float_S = 0b0100111,
  Float_R4_ADD = 0b1000011,
  Float_R4_SUB = 0b1000111,
  Float_R4_NMSUB = 0b1001011,
  Float_R4_NMADD = 0b1001111,
  Float_R = 0b1010011,
  Float_CSR = 0b1110011,
};

// The below might not be needed at all.
// enum FExtension {
//   FLW = 0b0000111, // Float I-type instruction
//   FSW = 0b0100111, // Float S-type instruction
//   // The below are Float R4-type instructions
//   FMADD_S = 0b1000011,
//   FMSUB_S = 0b1000111,
//   FNMSUB_S = 0b1001011,
//   FNMADD_S = 0b1001111,
//   // The below are Float R-type instructions
//   FADD_S = 0b1010011,
//   FSUB_S = 0b1010011,
//   FDIV_S = 0b1010011,
//   FSQRT_S = 0b1010011,
//   FSGNJ_S = 0b1010011,
//   FSGNJN_S = 0b1010011,
//   FSGNJX_S = 0b1010011,
//   FMIN_S = 0b1010011,
//   FMAX_S = 0b1010011,
//   FCVT_S_W = 0b1010011,
//   FCVT_S_WU = 0b1010011,
//   FMV_W_X = 0b1010011,
//   FEQ_S = 0b1010011,
//   FLT_S = 0b1010011,
//   FLE_S = 0b1010011,
//   FCLASS_S = 0b1010011,
//   FCVT_W_S = 0b1010011,
//   FCVT_WU_S = 0b1010011,
//   FMV_X_W = 0b1010011,
//   // The below instructions will not be implemented for now.
//   FRFLAGS = 0b1110011,
//   FSFLAGS = 0b1110011,
//   FSFLAGSI = 0b1110011,
//   FRRM = 0b1110011,
//   FSRM = 0b1110011,
//   FSRMI = 0b1110011,
//   FSCSR = 0b1110011,
//   FRCSR = 0b1110011,
// };

enum FExtension_Funct3 {
  FLW_Funct3 = 0b010,
  FSW_Funct3 = 0b010,
  FSGNJ_S_Funct3 = 0b000,
  FSGNJN_S_Funct3 = 0b001,
  FSGNJX_S_Funct3 = 0b010,
  FMIN_S_Funct3 = 0b000,
  FMAX_S_Funct3 = 0b001,
  FMV_X_W_Funct3 = 0b000,
  FEQ_S_Funct3 = 0b010,
  FLT_S_Funct3 = 0b001,
  FLE_S_Funct3 = 0b000,
  FCLASS_S_Funct3 = 0b001,
  FMV_W_X_Funct3 = 0b000,
  FRFLAGS_Funct3 = 0b010,
  FSFLAGS_Funct3 = 0b001,
  FSFLAGSI_Funct3 = 0b101,
  FRRM_Funct3 = 0b010,
  FSRM_Funct3 = 0b001,
  FSRMI_Funct3 = 0b101,
  FSCSR_Funct3 = 0b001,
  FRCSR_Funct3 = 0b010
};

enum FExtension_Funct7 {
  FADD_S_Funct7 = 0b0000000,
  FSUB_S_Funct7 = 0b0000100,
  FMUL_S_Funct7 = 0b0001000,
  FDIV_S_Funct7 = 0b0001100,
  FSQRT_S_Funct7 = 0b0101100,
  FSGNJ_S_Funct7 = 0b0010000,
  FSGNJN_S_Funct7 = 0b0010000,
  FSGNJX_S_Funct7 = 0b0010000,
  FMIN_S_Funct7 = 0b0010100,
  FMAX_S_Funct7 = 0b0010100,
  FCVT_W_S_Funct7 = 0b1100000,
  FCVT_WU_S_Funct7 = 0b1100000,
  FMV_X_W_Funct7 = 0b1110000,
  FEQ_S_Funct7 = 0b1010000,
  FLT_S_Funct7 = 0b1010000,
  FLE_S_Funct7 = 0b1010000,
  FCLASS_S_Funct7 = 0b1110000,
  FCVT_S_W_Funct7 = 0b1101000,
  FCVT_S_WU_Funct7 = 0b1101000,
  FMV_W_X_Funct7 = 0b1111000,
  FRFLAGS_Funct7 = 0,
  FSFLAGS_Funct7 = 0,
  FSFLAGSI_Funct7 = 0,
  FRRM_Funct7 = 0,
  FSRM_Funct7 = 0,
  FSRMI_Funct7 = 0,
  FSCSR_Funct7 = 0,
  FRCSR_Funct7 = 0
};

enum FExtension_Funct2 {
  FMADD_S_Funct2 = 0,
  FMSUB_S_Funct2 = 0,
  FNMSUB_S_Funct2 = 0,
  FNMADD_S_Funct2 = 0
};

enum FExtension_RS2 {
  FSQRT_S_RS2 = 0b00000,
  FCVT_W_S_RS2 = 0b00000,
  FCVT_WU_S_RS2 = 0b00001,
  FMV_X_W_RS2 = 0b00000,
  FCLASS_S_RS2 = 0b00000,
  FCVT_S_W_RS2 = 0b00000,
  FCVT_S_WU_RS2 = 0b00001,
  FMV_W_X_RS2 = 0b00000,
  FRFLAGS_RS2 = 0b00001,
  FSFLAGS_RS2 = 0b00001,
  FSFLAGSI_RS2 = 0b00001,
  FRRM_RS2 = 0b00010,
  FSRM_RS2 = 0b00010,
  FSRMI_RS2 = 0b00010,
  FSCSR_RS2 = 0b00011,
  FRCSR_RS2 = 0b00011
};

enum FExtension_RS1 {
  FRFLAGS_RS1 = 0b00000,
  FRRM_RS1 = 0b00000,
  FRCSR_RS1 = 0b00000
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
