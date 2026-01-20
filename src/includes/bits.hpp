#pragma once

#include <cstdint>
#include "system.hpp"

// This is used to get the type of instruction being executed
enum class ITYPES { R = 0, I = 1, J = 2, B = 3, U = 4, S = 5 };

// What part of the instruction is encoded in different parts
struct INST_BIT_SHIFT {
  // This is for the
  constexpr static uint8_t RD_SHIFT = 7;
  constexpr static uint8_t RS1_SHIFT = 15;
  constexpr static uint8_t RS2_SHIFT = 20;
  constexpr static uint8_t I_IMM_SHIFT = 20;
  constexpr static uint8_t U_IMM_SHIFT = 12;
  constexpr static uint8_t S_IMM_LOWER_SHIFT = 7;
  constexpr static uint8_t S_IMM_UPPER_SHIFT = 25;
  constexpr static uint8_t S_IMM_UPPER_L_SHIFT = 5;  

  // The J-type shifts
  constexpr static uint8_t J_SHIFT1 = 21;
  constexpr static uint8_t J_SHIFT2 = 20;
  constexpr static uint8_t J_SHIFT3 = 12;
  constexpr static uint8_t J_SHIFT4 = 31;

  // J-type shifts left to make the J-type immediate value.
  constexpr static uint8_t J_LSHIFT1 = 1;
  constexpr static uint8_t J_LSHIFT2 = 11;
  constexpr static uint8_t J_LSHIFT3 = 12;
  constexpr static uint8_t J_LSHIFT4 = 20;

  // B-type instructions
  constexpr static uint8_t B_SHIFT1 = 8;
  constexpr static uint8_t B_SHIFT2 = 25;
  constexpr static uint8_t B_SHIFT3 = 7;

  constexpr static uint8_t B_L_SHIFT1 = 1;
  constexpr static uint8_t B_L_SHIFT2 = 5;
  constexpr static uint8_t B_L_SHIFT3 = 11;

  // This is for decoding the instructions
  constexpr static uint8_t OPCODE_SHIFT = 0;
  constexpr static uint8_t FUNCT3_SHIFT = 12;
  constexpr static uint8_t FUNCT7_SHIFT = 25;

};

// TODO: Generate the indices using X-macros
// The index for the dispatch tables.
enum class RFuncIndex { // R-type function indices
  ADD = 0,
  SUB = 1,
  XOR = 2,
  OR = 3,
  AND = 4,
  SLL = 5,
  SRL = 6,
  SRA = 7,
  SLT = 8,
  SLTU = 9,
  // Here onwards are the integer multiplication and division extension
  // instructions.
  MUL = 10,
  MULH = 11,
  MULSU = 12,
  MULHU = 13,
  DIV = 14,
  DIVU = 15,
  REM = 16,
  REMU = 17
};

// The index for the dispatch tables.
enum class IFuncIndex { // R-type function indices
  ADD = 0,
  OR = 1,
  XOR = 2,
  AND = 3,
  SLLI = 4,
  SRLI = 5,
  SRAI = 6,
  SLTI = 7,
  SLTIU = 8,
  // The load instructions
  LB = 9,
  LH = 10,
  LW = 11,
  LBU = 12,
  LHU = 13,
  JALR = 14,
  ECALL = 15,
  // TODO:
  EBREAK = 16
};

enum class SFuncIndex { SB = 0, SH = 1, SW = 2 };

enum class UFuncIndex { LUI = 0, AUIPC = 1 };

enum class JFuncIndex { JAL = 0 };

enum class BFuncIndex { BEQ=0, BNE, BLT, BGE, BLTU, BGEU };

// The return type from Decode and into the execute stage.
struct Type_Index {
  ITYPES type; // The type of instruction to execute
  union {
    RFuncIndex rindex;
    IFuncIndex iindex;
    SFuncIndex sindex;
    UFuncIndex uindex;
    JFuncIndex jindex;
    BFuncIndex bindex;
  }; // The lookup index for the function ptr to execute.
};

// The masks to get the required operands to work with
struct Masks {
  constexpr static uword_t RD_MASK = 0b11111;
  constexpr static uword_t RS1_MASK = RD_MASK;
  constexpr static uword_t RS2_MASK = RD_MASK;
  constexpr static uword_t I_IMM_MASK = 0b111111111111;
  constexpr static uword_t S_IMM_LOWER_MASK = 0b11111;
  constexpr static uword_t S_IMM_UPPER_MASK = 0b1111111;
  constexpr static uword_t U_IMM_MASK = 0xFFFFF;
  constexpr static uword_t J_IMM_MASK1 = 0b1111111111;
  constexpr static uword_t J_IMM_MASK2 = 0b1;
  constexpr static uword_t J_IMM_MASK3 = 0b11111111;
  constexpr static uword_t J_IMM_MASK4 = 0b1;
  constexpr static uword_t B_IMM_MASK1 = 0b1111;
  constexpr static uword_t B_IMM_MASK2 = 0b111111;
  constexpr static uword_t B_IMM_MASK3 = 0b1;

  // Opcode masks
  constexpr static uint8_t FUNC3_MASK = 0b111;
  constexpr static uint8_t FUNC7_MASK = 0b1111111;  
};

