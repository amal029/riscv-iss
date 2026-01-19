#pragma once

#include "../../lib/includes/syscodes.h"
#include "bits.hpp"
#include "system.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
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
  static inline constexpr void MULU(word_t *reg, uint8_t rd, uint8_t rs1,
                                    uint8_t rs2) {
    static_assert(WORD == 4, "Only 32-bit mul/div supported for now");
    // XXX: Check that this is what the semantics are?
    reg[rd] = ((uint64_t)reg[rs1] * (uint64_t)reg[rs2]) >> 32;
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
  constexpr static void ADD(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                            word_t imm, size_t *PC, bool *PC_Change) {
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
  constexpr static void OR(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                           word_t imm, size_t *PC, bool *PC_Change) {
    reg[rd] = reg[rs1] | imm;
  }

  [[gnu::always_inline]]
  constexpr static void XOR(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                            word_t imm, size_t *PC, bool *PC_Change) {
    reg[rd] = reg[rs1] | imm;
  }

  [[gnu::always_inline]]
  constexpr static void AND(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                            word_t imm, size_t *PC, bool *PC_Change) {
    reg[rd] = reg[rs1] & imm;
  }

  [[gnu::always_inline]]
  constexpr static void SLLI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = reg[rs1] << (imm & 0b11111);
  }

  [[gnu::always_inline]]
  constexpr static void SLRI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = (word_t)((uword_t)reg[rs1]) >> (imm & 0b11111);
  }

  [[gnu::always_inline]]
  constexpr static void SARI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = reg[rs1] >> (imm & 0b11111);
  }

  [[gnu::always_inline]]
  constexpr static void SLTI(word_t *reg, uint8_t rd, uint8_t rs1,
                             uint8_t *DMEM, word_t imm, size_t *PC,
                             bool *PC_Change) {
    reg[rd] = rs1 < imm ? 1 : 0;
  }

  [[gnu::always_inline]]
  constexpr static void SLTIU(word_t *reg, uint8_t rd, uint8_t rs1,
                              uint8_t *DMEM, word_t imm, size_t *PC,
                              bool *PC_Change) {
    reg[rd] = reg[rs1] < (uword_t)imm ? 1 : 0;
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
    // fprintf("reg: %d\n", rs1);
  }

  [[gnu::always_inline]]
  static void LHU(word_t *reg, uint8_t rd, uint8_t rs1, uint8_t *DMEM,
                  word_t imm, size_t *PC, bool *PC_Change) {
    // bzero(reg + rd, WORD);
    std::memcpy(reg + rd, DMEM + reg[rs1] + imm, WORD / 2);
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
      printf("%d,%x,%d", fd, address, len);
#endif
      write(fd, DMEM + address, len);
      break;
    case SYS_EXIT:
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

  Execute(word_t *rf, uint8_t *DMEM, size_t *PC, bool *PC_Change)
      : REGFILE(rf), DMEM(DMEM), PC(PC), PC_Change(PC_Change) {}
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
      rops[static_cast<size_t>(tyi.rindex)](REGFILE, rd, rs1, rs2);
      break;
    }
    case ITYPES::I: {
      // Get the immediate operand and then extend it to signed bit
      word_t imm = (inst >> INST_BIT_SHIFT::I_IMM_SHIFT) & Masks::I_IMM_MASK;
      imm = sign_extend_imm(inst, imm);
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      uint8_t rs1 = ((inst >> INST_BIT_SHIFT::RS1_SHIFT) & Masks::RS1_MASK);
      iops[static_cast<size_t>(tyi.iindex)](REGFILE, rd, rs1, DMEM, imm, PC,
                                            PC_Change);
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
      bops[static_cast<size_t>(tyi.bindex)](REGFILE, rs1, rs2, PC, imm,
                                            PC_Change);
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
      jops[static_cast<size_t>(tyi.jindex)](REGFILE, rd, PC, imm, PC_Change);
      break;
    }
    case ITYPES::U: {
      uint8_t rd = ((inst >> INST_BIT_SHIFT::RD_SHIFT) & Masks::RD_MASK);
      word_t imm = inst & (Masks::U_IMM_MASK << INST_BIT_SHIFT::U_IMM_SHIFT);
#ifdef DEBUG
      std::cout << "U-type immediate: " << (word_t)imm << "\n";
#endif
      uops[static_cast<size_t>(tyi.uindex)](REGFILE, rd, *PC, imm);
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
      sops[static_cast<size_t>(tyi.sindex)](REGFILE, rs1, rs2, DMEM, imm);
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
  uint8_t *DMEM;
  size_t *PC;
  bool *PC_Change;
  // TODO: Generate these tables using X-macros
  constexpr static ROperations rops[18]{
      RFuncs::ADD,  RFuncs::SUB,  RFuncs::XOR,   RFuncs::OR,   RFuncs::AND,
      RFuncs::SLL,  RFuncs::SRL,  RFuncs::SRA,   RFuncs::SLT,  RFuncs::SLTU,
      RFuncs::MUL,  RFuncs::MULH, RFuncs::MULSU, RFuncs::MULU, RFuncs::DIV,
      RFuncs::DIVU, RFuncs::REM,  RFuncs::REMU};
  constexpr static IOperations iops[16]{
      IFuncs::ADD,  IFuncs::OR,   IFuncs::XOR,  IFuncs::AND,   IFuncs::SLLI,
      IFuncs::SLRI, IFuncs::SARI, IFuncs::SLTI, IFuncs::SLTIU, IFuncs::LB,
      IFuncs::LH,   IFuncs::LW,   IFuncs::LBU,  IFuncs::LHU,   IFuncs::JALR, IFuncs::ECALL};
  constexpr static SOperations sops[3]{SFuncs::SB, SFuncs::SH, SFuncs::SW};
  constexpr static UOperations uops[2]{UFuncs::LUI, UFuncs::AUIPC};
  constexpr static JOperations jops[2]{JFuncs::JAL};
  constexpr static BOperations bops[6]{BFuncs::BEQ, BFuncs::BNE,  BFuncs::BLT,
                                       BFuncs::BGE, BFuncs::BLTU, BFuncs::BGEU};
};
