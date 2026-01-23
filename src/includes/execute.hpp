#pragma once

#include "../../lib/includes/syscodes.h"
#include "bits.hpp"
#include "system.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
// #include <iostream>
#include <iostream>
#include <string.h>
#include <unistd.h>

// The R-type functions
struct RFuncs {
  [[gnu::always_inline]]
  static inline constexpr void ADD(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    reg[rd] = reg[rs1] + reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void SUB(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    reg[rd] = reg[rs1] - reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void XOR(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    reg[rd] = reg[rs1] ^ reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void OR(word_t *reg, uint8_t rd, uint8_t rs1,
                                  uint8_t rs2) {
    reg[rd] = reg[rs1] | reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void AND(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    reg[rd] = reg[rs1] & reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void SLL(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    reg[rd] = reg[rs1] << reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void SRA(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    reg[rd] = reg[rs1] >> reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void SRL(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    reg[rd] = (word_t)((uword_t)reg[rs1]) >> reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void SLT(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    reg[rd] = reg[rs1] < reg[rs2] ? 1 : 0;
  }

  [[gnu::always_inline]]
  static inline constexpr void SLTU(word_t *reg, uint8_t rd, uint8_t rs1,
                                    uint8_t rs2) {
    // XXX: Check that this is what the semantics are?
    reg[rd] = (uword_t)reg[rs1] < (uword_t)reg[rs2] ? 1 : 0;
  }

  [[gnu::always_inline]]
  static inline constexpr void MUL(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = ((int64_t)reg[rs1] * (int64_t)reg[rs2]) & 0xFFFFFFFF;
  }

  [[gnu::always_inline]]
  static inline constexpr void MULH(word_t *reg, uint8_t rd, uint8_t rs1,
                                    uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = ((int64_t)reg[rs1] * (int64_t)reg[rs2]) >> 32;
  }

  [[gnu::always_inline]]
  static inline constexpr void MULSU(word_t *reg, uint8_t rd, uint8_t rs1,
                                     uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = ((int64_t)reg[rs1] * (uint64_t)reg[rs2]) >> 32;
  }

  [[gnu::always_inline]]
  static inline constexpr void MULHU(word_t *reg, uint8_t rd, uint8_t rs1,
                                     uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = (uword_t)(((uint64_t)reg[rs1] * (uint64_t)reg[rs2]) >> 32);
  }

  [[gnu::always_inline]]
  static inline constexpr void DIV(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = reg[rs1] / reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void DIVU(word_t *reg, uint8_t rd, uint8_t rs1,
                                    uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = (uword_t)reg[rs1] / (uword_t)reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void REM(word_t *reg, uint8_t rd, uint8_t rs1,
                                   uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = reg[rs1] % reg[rs2];
  }

  [[gnu::always_inline]]
  static inline constexpr void REMU(word_t *reg, uint8_t rd, uint8_t rs1,
                                    uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = (uword_t)reg[rs1] % (uword_t)reg[rs2];
  }
};

struct IFuncs {
  [[gnu::always_inline]]
  constexpr static void ADDI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = reg[rs1] + imm;
  }

  [[gnu::always_inline]]
  constexpr static void JALR(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = *PC + 4;
    *PC = (reg[rs1] + imm);
    // Setting the LSB to 0 as stated in the official documentation
    *PC = WORD == 4 ? *PC & 0xFFFFFFFE : *PC & 0xFFFFFFFFFFFFFFFE;
    *PC_Change = true;
  }

  [[gnu::always_inline]]
  constexpr static void ORI(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                            word_t imm, size_t *PC, bool *PC_Change) {
    reg[rd] = reg[rs1] | imm;
  }

  [[gnu::always_inline]]
  constexpr static void XORI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = reg[rs1] ^ imm;
  }

  [[gnu::always_inline]]
  constexpr static void ANDI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = reg[rs1] & imm;
  }

  [[gnu::always_inline]]
  constexpr static void SLLI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = reg[rs1] << (imm & 0b11111);
  }

  [[gnu::always_inline]]
  constexpr static void SRLI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = (word_t)(((uword_t)reg[rs1]) >> (imm & 0b11111));
  }

  [[gnu::always_inline]]
  constexpr static void SRAI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = reg[rs1] >> (imm & 0b11111);
  }

  [[gnu::always_inline]]
  constexpr static void SLTI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = reg[rs1] < imm ? 1 : 0;
  }

  [[gnu::always_inline]]
  constexpr static void SLTIU(word_t *reg, uint8_t rd, uint8_t rs1,
                              uint8_t *DMEM, word_t imm, size_t *PC,
                              bool *PC_Change) {
    reg[rd] = (uword_t)reg[rs1] < (uword_t)imm ? 1 : 0;
  }

  [[gnu::always_inline]]
  static void LW(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                 word_t imm, size_t *PC, bool *PC_Change) {
    // bzero(reg + rd, WORD);
    std::memcpy(reg + rd, DMEM + reg[rs1] + imm, WORD);
  }

  [[gnu::always_inline]]
  static void LH(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                 word_t imm, size_t *PC, bool *PC_Change) {
    // bzero(reg + rd, WORD);
    std::memcpy(reg + rd, DMEM + reg[rs1] + imm, WORD / 2);
    // TODO: This will not work for 64-bit architecture
    // Sign extend the number
    reg[rd] = ((reg[rd] >> ((WORD * 8 / 2) - 1)) == 0x0)
                  ? reg[rd]
                  : reg[rd] | 0xFFFF << (WORD / 2 * 8);
  }

  [[gnu::always_inline]]
  static void LB(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                 word_t imm, size_t *PC, bool *PC_Change) {
    // bzero(reg + rd, WORD);
    std::memcpy(reg + rd, DMEM + reg[rs1] + imm, 1);
    // Need to sign extend the number
    reg[rd] = ((reg[rd] >> 7) & 0x1) == 0x0
                  ? reg[rd]
                  : (WORD == 4 ? reg[rd] | 0xFFFFFF << 8
                               // XXX: This is for 64-bit architecture
                               : reg[rd] | 0xFFFFFFFFFFFFFF << 8);
  }

  [[gnu::always_inline]]
  static void LBU(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                  word_t imm, size_t *PC, bool *PC_Change) {
    // bzero(reg + rd, WORD);	// because registers are being shared here!
    std::memcpy(reg + rd, DMEM + reg[rs1] + imm, 1);
    // After this set all higher bytes to zero
    reg[rd] &= (WORD == 4) ? 0x000000FF : 0x00000000000000FF;
    // fprintf("reg: %d\n", rs1);
  }

  [[gnu::always_inline]]
  static void LHU(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                  word_t imm, size_t *PC, bool *PC_Change) {
    // bzero(reg + rd, WORD);
    std::memcpy(reg + rd, DMEM + reg[rs1] + imm, WORD / 2);
    reg[rd] &= (WORD == 4) ? 0x0000FFFF : 0x00000000FFFFFFFF;
  }

  [[gnu::always_inline]]
  static void ECALL(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                    word_t imm, size_t *PC, bool *PC_Change) {
    // XXX: Check the syscall number in register a7 (x17)
    int fd;
    word_t address;
    size_t len;
    switch (reg[17]) {
    case SYS_WRITE:
      // XXX: Here the fd is in a0, a1 has the address, and a2 is the
      // length of the buffer to write.
      fd = reg[10];
      address = reg[11];
      len = reg[12];
#ifdef DEBUG
      printf("%d,%x,%lu", fd, address, len);
#endif
      write(fd, DMEM + address, len);
      break;
    case SYS_EXIT:
      exit((int)reg[10]);
      break;
    case SYS_READ:
      fd = reg[10];
      address = reg[11];
      len = reg[12];
      read(fd, DMEM + address, len);
      break;
      break;
    }
  }
};

struct SFuncs {
  [[gnu::always_inline]]
  static void SB(word_t *reg, uint8_t rs1, uint8_t rs2, uint8_t *DMEM,
                 word_t imm) {
    std::memcpy(DMEM + reg[rs1] + imm, reg + rs2, 1);
  }

  [[gnu::always_inline]]
  static void SH(word_t *reg, uint8_t rs1, uint8_t rs2, uint8_t *DMEM,
                 word_t imm) {
    std::memcpy(DMEM + reg[rs1] + imm, reg + rs2, WORD / 2);
  }

  [[gnu::always_inline]]
  static void SW(word_t *reg, uint8_t rs1, uint8_t rs2, uint8_t *DMEM,
                 word_t imm) {
#ifdef DEBUG
    fprintf(stderr, "%d, %d, %x\n", imm, reg[rs1] + imm, reg[rs1] + imm);
    fprintf(stderr, "%x\n", reg[rs2]);
    fprintf(stderr, "%x\n", (word_t)DMEM[reg[rs1] + imm]);
#endif
    // DMEM[reg[rs1] + imm] = (word_t)reg[rs2];
    std::memcpy(DMEM + reg[rs1] + imm, reg + rs2, WORD);
#ifdef DEBUG
    fprintf(stderr, "%x\n", (word_t)DMEM[reg[rs1] + imm]);
#endif
  }
};

struct UFuncs {

  [[gnu::always_inline]]
  static void LUI(word_t *reg, uint8_t rd, size_t PC, word_t imm) {
#ifdef DEBUG
    fprintf(stderr, "LUI: %d, %x\n", imm, imm);
#endif
    reg[rd] = imm;
  }

  [[gnu::always_inline]]
  static void AUIPC(word_t *reg, uint8_t rd, size_t PC, word_t imm) {
    reg[rd] = PC + imm;
  }
};

struct JFuncs {
  [[gnu::always_inline]]
  static void JAL(word_t *reg, uint8_t rd, size_t *PC, word_t imm,
                  bool *PC_Change) {
    reg[rd] = *PC + 4;
    *PC += imm;
    *PC_Change = true;
  }
};

struct BFuncs {

  [[gnu::always_inline]]
  static void BEQ(word_t *reg, uint8_t rs1, uint8_t rs2, size_t *PC, word_t imm,
                  bool *PC_Change) {
    if (reg[rs1] == reg[rs2]) {
      *PC = *PC + imm;
      *PC_Change = true;
    }
    // *PC = reg[rs1] == reg[rs2] ? *PC += imm : *PC;
    // *PC_Change = true;
  }

  [[gnu::always_inline]]
  static void BNE(word_t *reg, uint8_t rs1, uint8_t rs2, size_t *PC, word_t imm,
                  bool *PC_Change) {
#ifdef DEBUG
    fprintf(stderr, "%x, %d\n", imm, imm);
    fprintf(stderr, "PC = %zx\n", *PC);
#endif
    if (reg[rs1] != reg[rs2]) {
      *PC = *PC + imm;
      *PC_Change = true;
    }
    // *PC = reg[rs1] != reg[rs2] ? *PC += imm : *PC;
    // *PC_Change = true;
#ifdef DEBUG
    fprintf(stderr, "BNE PC = %zx\n", *PC);
#endif
  }

  [[gnu::always_inline]]
  static void BLT(word_t *reg, uint8_t rs1, uint8_t rs2, size_t *PC, word_t imm,
                  bool *PC_Change) {
    if (reg[rs1] < reg[rs2]) {
      *PC += imm;
      *PC_Change = true;
    }
    // *PC = reg[rs1] < reg[rs2] ? *PC += imm : *PC;
    // *PC_Change = true;
  }

  [[gnu::always_inline]]
  static void BGE(word_t *reg, uint8_t rs1, uint8_t rs2, size_t *PC, word_t imm,
                  bool *PC_Change) {
    if (reg[rs1] >= reg[rs2]) {
      *PC += imm;
      *PC_Change = true;
    }
    // *PC = reg[rs1] >= reg[rs2] ? *PC += imm : *PC;
    // *PC_Change = true;
  }

  [[gnu::always_inline]]
  static void BLTU(word_t *reg, uint8_t rs1, uint8_t rs2, size_t *PC,
                   word_t imm, bool *PC_Change) {
    if ((uword_t)reg[rs1] < (uword_t)reg[rs2]) {
      *PC += imm;
      *PC_Change = true;
    }
    // *PC = (uword_t)reg[rs1] < (uword_t)reg[rs2] ? *PC += imm : *PC;
    // *PC_Change = true;
  }

  [[gnu::always_inline]]
  static void BGEU(word_t *reg, uint8_t rs1, uint8_t rs2, size_t *PC,
                   word_t imm, bool *PC_Change) {
    if ((uword_t)reg[rs1] >= (uword_t)reg[rs2]) {
      *PC += imm;
      *PC_Change = true;
    }
    // *PC = (uword_t)reg[rs1] >= (uword_t)reg[rs2] ? *PC += imm : *PC;
    // *PC_Change = true;
  }
};

// Floating point instructions
struct FExtension {
  [[gnu::always_inline]]
  static void FLW(fword_t *freg, word_t *reg, uint8_t rd, uint8_t rs1,
                  uint8_t *DMEM, word_t imm) {
    std::memcpy(freg + rd, DMEM + reg[rs1] + imm, WORD);
  }
  // Floating point instructions
  [[gnu::always_inline]]
  static void FSW(fword_t *freg, word_t *reg, uint8_t *DMEM, uint8_t rs1,
                  uint8_t rs2, word_t imm) {
    std::memcpy(DMEM + reg[rs1] + imm, freg + rs2, WORD);
  }

  // Floating point instructions
  [[gnu::always_inline]]
  static void FMADD_S(fword_t *freg, uint8_t rs1, uint8_t rs2, uint8_t rs3,
                      uint8_t rd) {
    freg[rd] = freg[rs1] * freg[rs2] + freg[rs3];
  }

  // Floating point instructions
  [[gnu::always_inline]]
  static void FMSUB_S(fword_t *freg, uint8_t rs1, uint8_t rs2, uint8_t rs3,
                      uint8_t rd) {
    freg[rd] = freg[rs1] * freg[rs2] - freg[rs3];
  }

  // Floating point instructions
  [[gnu::always_inline]]
  static void FNMADD_S(fword_t *freg, uint8_t rs1, uint8_t rs2, uint8_t rs3,
                       uint8_t rd) {
    freg[rd] = -freg[rs1] * freg[rs2] + freg[rs3];
  }

  [[gnu::always_inline]]
  static void FNMSUB_S(fword_t *freg, uint8_t rs1, uint8_t rs2, uint8_t rs3,
                       uint8_t rd) {
    freg[rd] = -freg[rs1] * freg[rs2] - freg[rs3];
  }

  [[gnu::always_inline]]
  static void FADD_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                     uint8_t rd) {
    freg[rd] = freg[rs1] + freg[rs2];
  }

  [[gnu::always_inline]]
  static void FSUB_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                     uint8_t rd) {
    freg[rd] = freg[rs1] - freg[rs2];
  }

  [[gnu::always_inline]]
  static void FMUL_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                     uint8_t rd) {
    freg[rd] = freg[rs1] * freg[rs2];
  }

  [[gnu::always_inline]]
  static void FDIV_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                     uint8_t rd) {
    freg[rd] = freg[rs1] / freg[rs2];
  }

  [[gnu::always_inline]]
  static void FSQRT_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                      uint8_t rd) {
    freg[rd] = std::sqrtf(freg[rs1]);
  }

  [[gnu::always_inline]]
  static void FSGNJ_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                      uint8_t rd) {
    int8_t sgn = std::signbit(freg[rs2]) == false ? 1 : -1;
#ifdef DEBUG
    std::cout << "FSGNJ_S: " << freg[rs1]
              << "abs value: " << std::abs(freg[rs1]) << "sgn: " << sgn << "\n";
#endif
    freg[rd] = std::abs(freg[rs1]) * sgn;
#ifdef DEBUG
    std::cout << "FSGNJ_S result: " << freg[rd] << "\n";
#endif
  }

  [[gnu::always_inline]]
  static void FSGNJN_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                       uint8_t rd) {
    int8_t sgn = std::signbit(freg[rs2]) == false ? 1 : -1;
#ifdef DEBUG
    std::cout << "FSGNJN_S: " << std::abs(freg[rs1]) << "sgn: " << sgn << "\n";
#endif
    freg[rd] = std::abs(freg[rs1]) * -sgn;
  }

  [[gnu::always_inline]]
  static void FSGNJX_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                       uint8_t rd) {
    int8_t sgn = std::signbit(freg[rs2]) == false ? 1 : -1;
    freg[rd] = freg[rs1] * sgn;
  }

  [[gnu::always_inline]]
  static void FMIN_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                     uint8_t rd) {
    freg[rd] = std::min(freg[rs1], freg[rs2]);
  }

  [[gnu::always_inline]]
  static void FMAX_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                     uint8_t rd) {
    freg[rd] = std::max(freg[rs1], freg[rs2]);
  }

  [[gnu::always_inline]]
  static void FCVT_S_W(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                       uint8_t rd) {
    freg[rd] = (fword_t)reg[rs1];
  }

  [[gnu::always_inline]]
  static void FCVT_S_WU(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                        uint8_t rd) {
    freg[rd] = (fword_t)reg[rs1];
  }

  [[gnu::always_inline]]
  static void FCVT_W_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                       uint8_t rd) {
#ifdef DEBUG
    std::cout << (float)freg[rs1] << "\n";
    std::cout << (word_t)freg[rs1] << "\n";
#endif
    reg[rd] = (word_t)freg[rs1];
  }

  [[gnu::always_inline]]
  static void FCVT_WU_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                        uint8_t rd) {
    reg[rd] = (uword_t)freg[rs1];
  }

  [[gnu::always_inline]]
  static void FMV_X_W(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                      uint8_t rd) {
    static_assert(sizeof(freg[rs1]) == WORD, "Only 32 bit handled right now");
    std::memcpy(reg + rd, freg + rs1, WORD);
  }

  [[gnu::always_inline]]
  static void FMV_W_X(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                      uint8_t rd) {
    std::memcpy(freg + rd, reg + rs1, WORD);
  }

  [[gnu::always_inline]]
  static void FEQ_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                    uint8_t rd) {
#ifdef DEBUG
    std::cout << "FEQ_S: " << (freg[rs1] == freg[rs2]);
#endif
    reg[rd] = freg[rs1] == freg[rs2];
  }

  [[gnu::always_inline]]
  static void FLT_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                    uint8_t rd) {
#ifdef DEBUG
    std::cout << "FLT_S: " << (freg[rs1] < freg[rs2]) << "\n";
#endif
    reg[rd] = freg[rs1] < freg[rs2];
  }

  [[gnu::always_inline]]
  static void FLE_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                    uint8_t rd) {
    reg[rd] = freg[rs1] <= freg[rs2];
  }

  [[gnu::always_inline]]
  static void FCLASS_S(fword_t *freg, word_t *reg, uint8_t rs1, uint8_t rs2,
                       uint8_t rd) {
    word_t res = 0b0000000001;
    fword_t w = freg[rs1];
    // Check for different classes
    switch (std::fpclassify(w)) {
    case FP_INFINITE: {
      if (std::signbit(w))
        reg[rd] = res; // -oo
      else
        reg[rd] = res << 7; // +oo
      break;
    }
    case FP_SUBNORMAL: {
      if (std::signbit(w))
        reg[rd] = res << 2; // -SUBNORMAL
      else
        reg[rd] = res << 5; // +SUBNORMAL
      break;
    }
    case FP_NORMAL: {
      if (std::signbit(w))
        reg[rd] = res << 1; // -NORMAL
      else
        reg[rd] = res << 6; // +NORMAL
      break;
    }
    case FP_ZERO: {
      if (std::signbit(w))
        reg[rd] = res << 3; // -0
      else
        reg[rd] = res << 4; // +0
      break;
    }
    case FP_NAN: {
      reg[rd] = res << 9;
      break;
    } // We never have a signalling NaN
    }
  }
};

struct Execute {

  // Sign extend the number
  constexpr static inline word_t sign_extend_imm(uint32_t inst, word_t x) {
    uint8_t sign = (inst >> 31 & 0x1); // instruction is always 32 bits
    x = (sign == 0) ? x : (WORD == 4 ? x | 0xFFFFF000 : x | 0xFFFFFFFFFFFFF000);
    return x;
  }

  constexpr static inline word_t sign_extend_j(uint32_t inst, word_t x) {
    uint8_t sign = (inst >> 31 & 0x1); // instruction is always 32 bits
    x = (sign == 0) ? x : (WORD == 4 ? x | 0xFFF00000 : x | 0xFFFFFFFFFFF00000);
    return x;
  }

  Execute(word_t *rf, fword_t *frf, uint8_t *DMEMa, size_t *PCa,
          bool *PC_Changea)
      : REGFILE(rf), REGFILE_FLOAT(frf), DMEM(DMEMa), PC(PCa),
        PC_Change(PC_Changea) {}
  // This class executes the given instruction using function pointer
  // dispatch.
  void execute(Type_Index tyi, uint32_t inst) {
    switch (tyi.type) {
    case ITYPES::R: {
      // XXX: Get the source and destination register indices from the
      // instruction
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      uint8_t rs1 = ((inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK);
      uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
      rops[tyi.rindex](REGFILE, rd, rs1, rs2);
      break;
    }
    case ITYPES::I: {
      // Get the immediate operand and then extend it to signed bit
      word_t imm = (inst >> INST_BIT_SHIFT::I_IMM_SHIFT) & Masks::I_IMM_MASK;
      imm = sign_extend_imm(inst, imm);
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      uint8_t rs1 = ((inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK);
      iops[tyi.iindex](REGFILE, rd, rs1, DMEM, imm, PC, PC_Change);
      break;
    }
    case ITYPES::B: {
      uint8_t rs1 = (inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK;
      uint8_t rs2 = (inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK;
      // Now make the B-type immediate
      word_t imm1 = ((inst >> INST_BIT_SHIFT::B_SHIFT1) & Masks::B_IMM_MASK1)
                    << INST_BIT_SHIFT::B_L_SHIFT1;
      word_t imm2 = ((inst >> INST_BIT_SHIFT::B_SHIFT2) & Masks::B_IMM_MASK2)
                    << INST_BIT_SHIFT::B_L_SHIFT2;
      word_t imm3 = ((inst >> INST_BIT_SHIFT::B_SHIFT3) & Masks::B_IMM_MASK3)
                    << INST_BIT_SHIFT::B_L_SHIFT3;
      word_t imm = imm1 | imm2 | imm3;
      // Now sign extend this immediate value
      imm = sign_extend_imm(inst, imm); // we can use this here
#ifdef DEBUG
      std::cerr << std::bitset<32>{(unsigned long long)imm1} << "\n";
      std::cerr << std::bitset<32>{(unsigned long long)imm2} << "\n";
      std::cerr << std::bitset<32>{(unsigned long long)imm3} << "\n";
      fprintf(stderr, "%d, %d\n", rs1, rs2);
      std::cerr << std::bitset<32>{(unsigned long long)inst} << "\n";
      std::cerr << std::bitset<32>{(unsigned long long)imm} << "\n";
#endif
      bops[tyi.bindex](REGFILE, rs1, rs2, PC, imm, PC_Change);
      break;
    }
    case ITYPES::J: {
      // First get the rd
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      // get the immediate value -- not straight forward
      word_t imm1 = ((inst >> INST_BIT_SHIFT::J_SHIFT1) & Masks::J_IMM_MASK1)
                    << INST_BIT_SHIFT::J_LSHIFT1; // the bits 0..10
      word_t imm2 = ((inst >> INST_BIT_SHIFT::J_SHIFT2) & Masks::J_IMM_MASK2)
                    << INST_BIT_SHIFT::J_LSHIFT2; // 11th bit
      word_t imm3 = ((inst >> INST_BIT_SHIFT::J_SHIFT3) & Masks::J_IMM_MASK3)
                    << INST_BIT_SHIFT::J_LSHIFT3; // bits 12..19
      word_t imm4 = ((inst >> INST_BIT_SHIFT::J_SHIFT4) & Masks::J_IMM_MASK4)
                    << INST_BIT_SHIFT::J_LSHIFT4; // the 20..31 bits
      word_t imm = imm1 | imm2 | imm3 | imm4;
      // sign extend the immediate number
      imm = sign_extend_j(inst, imm);
      jops[tyi.jindex](REGFILE, rd, PC, imm, PC_Change);
      break;
    }
    case ITYPES::U: {
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      word_t imm = inst & (Masks::U_IMM_MASK << INST_BIT_SHIFT::U_IMM_SHIFT);
#ifdef DEBUG
      std::cout << "U-type immediate: " << (word_t)imm << "\n";
#endif
      uops[tyi.uindex](REGFILE, rd, *PC, imm);
      break;
    }
    case ITYPES::S: {
      uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
      uint8_t rs1 = ((inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK);
#ifdef DEBUG
      std::cout << "rs2: " << (uint32_t)rs2 << " rs1: " << (uint32_t)rs1
                << "\n";
#endif
      // Now get the immediate value and sign extend it.
      word_t tmp1 =
          (inst >> INST_BIT_SHIFT::S_IMM_LOWER_SHIFT) & Masks::S_IMM_LOWER_MASK;
      word_t tmp2 =
          (inst >> INST_BIT_SHIFT::S_IMM_UPPER_SHIFT) & Masks::S_IMM_UPPER_MASK;
      word_t imm = tmp1 | (tmp2 << INST_BIT_SHIFT::S_IMM_UPPER_L_SHIFT);
      imm = sign_extend_imm(inst, imm);
#ifdef DEBUG
      std::cout << "S-type immediate value: " << (int32_t)imm << "\n";
#endif
      sops[tyi.sindex](REGFILE, rs1, rs2, DMEM, imm);
      break;
    }
    case ITYPES::FloatI: {
      // Get the immediate operand and then extend it to signed bit
      word_t imm = (inst >> INST_BIT_SHIFT::I_IMM_SHIFT) & Masks::I_IMM_MASK;
      imm = sign_extend_imm(inst, imm);
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      uint8_t rs1 = ((inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK);
      fiops[tyi.iindex](REGFILE_FLOAT, REGFILE, rd, rs1, DMEM, imm);
      break;
    }
    case ITYPES::FloatS: {
      uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
      uint8_t rs1 = ((inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK);
#ifdef DEBUG
      std::cout << "rs2: " << (uint32_t)rs2 << " rs1: " << (uint32_t)rs1
                << "\n";
#endif
      // Now get the immediate value and sign extend it.
      word_t tmp1 =
          (inst >> INST_BIT_SHIFT::S_IMM_LOWER_SHIFT) & Masks::S_IMM_LOWER_MASK;
      word_t tmp2 =
          (inst >> INST_BIT_SHIFT::S_IMM_UPPER_SHIFT) & Masks::S_IMM_UPPER_MASK;
      word_t imm = tmp1 | (tmp2 << INST_BIT_SHIFT::S_IMM_UPPER_L_SHIFT);
      imm = sign_extend_imm(inst, imm);
#ifdef DEBUG
      std::cout << "S-type immediate value: " << (int32_t)imm << "\n";
#endif
      fsops[tyi.sindex](REGFILE_FLOAT, REGFILE, DMEM, rs1, rs2, imm);
      break;
    }
    case ITYPES::FloatR: {
      uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
      uint8_t rs1 = ((inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK);
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      frops[tyi.frindex](REGFILE_FLOAT, REGFILE, rs1, rs2, rd);
      break;
    }
    case ITYPES::FloatR4: {
      uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
      uint8_t rs1 = ((inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK);
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      uint8_t rs3 = ((inst >> INST_BIT_SHIFT::R3_SHIFT) & Masks::RS3_MASK);
      fr4ops[tyi.fr4index](REGFILE_FLOAT, rs1, rs2, rs3, rd);
      break;
    }
    case ITYPES::FloatCSR: {
      std::cout << "Float CSR instructions not yet implemented\n";
      exit(1);
      break;
    }
    }
  }
  // Now give the function pointer table for each of the different types.
  using ROperations = void (*)(word_t *reg, uint8_t rd, uint8_t rs1,
                               uint8_t rs2);

  using IOperations = void (*)(word_t *reg, uint8_t rd, uint8_t rs1,
                               uint8_t *DMEM, word_t imm, std::size_t *PC,
                               bool *PC_Change);

  using FIOperations = void (*)(fword_t *freg, word_t *reg, uint8_t rd,
                                uint8_t rs1, uint8_t *DMEM, word_t imm);

  using FR4Operations = void (*)(fword_t *freg, uint8_t rs1, uint8_t rs2,
                                 uint8_t rs3, uint8_t rd);

  using FROperations = void (*)(fword_t *freg, word_t *reg, uint8_t rs1,
                                uint8_t rs2, uint8_t rd);

  using FSOperations = void (*)(fword_t *freg, word_t *reg, uint8_t *DMEM,
                                uint8_t rs1, uint8_t rs2, word_t imm);

  using SOperations = void (*)(word_t *reg, uint8_t rs1, uint8_t rs2,
                               uint8_t *DMEM, word_t imm);

  using UOperations = void (*)(word_t *reg, uint8_t rd, size_t PC, word_t imm);
  using JOperations = void (*)(word_t *reg, uint8_t rd, size_t *PC, word_t imm,
                               bool *PC_Change);
  using BOperations = void (*)(word_t *reg, uint8_t rs1, uint8_t rs2,
                               size_t *PC, word_t imm, bool *PC_Change);
  void setPC_Change() { *PC_Change = false; }
  bool getPC_Change() const { return *PC_Change; }
  void setRegFile(uint8_t index, int value) {
    std::memset(REGFILE + index, value, WORD);
  }

private:
  word_t *REGFILE;
  fword_t *REGFILE_FLOAT;
  uint8_t *DMEM;
  size_t *PC;
  bool *PC_Change;

  // XXX: This will only allow compiling with clang++ and g++ MSVC does
  // not support this array designated initialization.
  constexpr static ROperations rops[]{
      [RFuncIndex::ADD] = RFuncs::ADD,     [RFuncIndex::SUB] = RFuncs::SUB,
      [RFuncIndex::XOR] = RFuncs::XOR,     [RFuncIndex::OR] = RFuncs::OR,
      [RFuncIndex::AND] = RFuncs::AND,     [RFuncIndex::SLL] = RFuncs::SLL,
      [RFuncIndex::SRL] = RFuncs::SRL,     [RFuncIndex::SRA] = RFuncs::SRA,
      [RFuncIndex::SLT] = RFuncs::SLT,     [RFuncIndex::SLTU] = RFuncs::SLTU,
      [RFuncIndex::MUL] = RFuncs::MUL,     [RFuncIndex::MULH] = RFuncs::MULH,
      [RFuncIndex::MULSU] = RFuncs::MULSU, [RFuncIndex::MULHU] = RFuncs::MULHU,
      [RFuncIndex::DIV] = RFuncs::DIV,     [RFuncIndex::DIVU] = RFuncs::DIVU,
      [RFuncIndex::REM] = RFuncs::REM,     [RFuncIndex::REMU] = RFuncs::REMU};
  static_assert(RFuncIndex::NUM_STATES_R == sizeof(rops) / sizeof(rops[0]),
                "Not all I-type operations defined\n");

  constexpr static IOperations iops[]{
      [IFuncIndex::ADDI] = IFuncs::ADDI,   [IFuncIndex::ORI] = IFuncs::ORI,
      [IFuncIndex::XORI] = IFuncs::XORI,   [IFuncIndex::ANDI] = IFuncs::ANDI,
      [IFuncIndex::SLLI] = IFuncs::SLLI,   [IFuncIndex::SRLI] = IFuncs::SRLI,
      [IFuncIndex::SRAI] = IFuncs::SRAI,   [IFuncIndex::SLTI] = IFuncs::SLTI,
      [IFuncIndex::SLTIU] = IFuncs::SLTIU, [IFuncIndex::LB] = IFuncs::LB,
      [IFuncIndex::LH] = IFuncs::LH,       [IFuncIndex::LW] = IFuncs::LW,
      [IFuncIndex::LBU] = IFuncs::LBU,     [IFuncIndex::LHU] = IFuncs::LHU,
      [IFuncIndex::JALR] = IFuncs::JALR,   [IFuncIndex::ECALL] = IFuncs::ECALL};
  static_assert(IFuncIndex::NUM_STATES_I == sizeof(iops) / sizeof(iops[0]),
                "Not all I-type operations defined\n");

  constexpr static SOperations sops[]{[SFuncIndex::SB] = SFuncs::SB,
                                      [SFuncIndex::SH] = SFuncs::SH,
                                      [SFuncIndex::SW] = SFuncs::SW};
  static_assert(SFuncIndex::NUM_STATES_S == sizeof(sops) / sizeof(sops[0]),
                "Not all S-type operations defined\n");

  constexpr static UOperations uops[]{[UFuncIndex::LUI] = UFuncs::LUI,
                                      [UFuncIndex::AUIPC] = UFuncs::AUIPC};
  static_assert(UFuncIndex::NUM_STATES_U == sizeof(uops) / sizeof(uops[0]),
                "Not all U-type operations defined\n");

  constexpr static JOperations jops[]{[JFuncIndex::JAL] = JFuncs::JAL};
  static_assert(JFuncIndex::NUM_STATES_J == sizeof(jops) / sizeof(jops[0]),
                "Not all J-type operations defined\n");

  constexpr static BOperations bops[]{
      [BFuncIndex::BEQ] = BFuncs::BEQ,   [BFuncIndex::BNE] = BFuncs::BNE,
      [BFuncIndex::BLT] = BFuncs::BLT,   [BFuncIndex::BGE] = BFuncs::BGE,
      [BFuncIndex::BLTU] = BFuncs::BLTU, [BFuncIndex::BGEU] = BFuncs::BGEU};
  static_assert(BFuncIndex::NUM_STATES_B == sizeof(bops) / sizeof(bops[0]),
                "Not all B-type operations defined\n");

  constexpr static FIOperations fiops[]{[Float_IIndex::FLW] = FExtension::FLW};
  static_assert(Float_IIndex::NUM_Float_I == sizeof(fiops) / sizeof(fiops[0]),
                "Not all Float I-type operations defined\n");

  constexpr static FSOperations fsops[]{[Float_SIndex::FSW] = FExtension::FSW};
  static_assert(Float_SIndex::NUM_Float_S == sizeof(fsops) / sizeof(fsops[0]),
                "Not all Float S-type operations defined\n");

  constexpr static FR4Operations fr4ops[]{
      [Float_R4Index::FMADD_S] = FExtension::FMADD_S,
      [Float_R4Index::FMSUB_S] = FExtension::FMSUB_S,
      [Float_R4Index::FNMSUB_S] = FExtension::FNMSUB_S,
      [Float_R4Index::FNMADD_S] = FExtension::FNMADD_S};
  static_assert(Float_R4Index::NUM_Float_R4 ==
                    sizeof(fr4ops) / sizeof(fr4ops[0]),
                "Not all Float R4-type operations defined\n");

  // FRoperations here
  constexpr static FROperations frops[]{
      [Float_RIndex::FADD_S] = FExtension::FADD_S,
      [Float_RIndex::FSUB_S] = FExtension::FSUB_S,
      [Float_RIndex::FMUL_S] = FExtension::FMUL_S,
      [Float_RIndex::FDIV_S] = FExtension::FDIV_S,
      [Float_RIndex::FSQRT_S] = FExtension::FSQRT_S,
      [Float_RIndex::FSGNJ_S] = FExtension::FSGNJ_S,
      [Float_RIndex::FSGNJN_S] = FExtension::FSGNJN_S,
      [Float_RIndex::FSGNJX_S] = FExtension::FSGNJX_S,
      [Float_RIndex::FMIN_S] = FExtension::FMIN_S,
      [Float_RIndex::FMAX_S] = FExtension::FMAX_S,
      [Float_RIndex::FCVT_S_W] = FExtension::FCVT_S_W,
      [Float_RIndex::FCVT_S_WU] = FExtension::FCVT_S_WU,
      [Float_RIndex::FMV_W_X] = FExtension::FMV_W_X,
      [Float_RIndex::FMV_X_W] = FExtension::FMV_X_W,
      [Float_RIndex::FEQ_S] = FExtension::FEQ_S,
      [Float_RIndex::FLT_S] = FExtension::FLT_S,
      [Float_RIndex::FLE_S] = FExtension::FLE_S,
      [Float_RIndex::FCLASS_S] = FExtension::FCLASS_S,
      [Float_RIndex::FCVT_W_S] = FExtension::FCVT_W_S,
      [Float_RIndex::FCVT_WU_S] = FExtension::FCVT_WU_S};
  static_assert(Float_RIndex::NUM_Float_R == sizeof(frops) / sizeof(frops[0]),
                "Not all Float R4-type operations defined\n");
};
