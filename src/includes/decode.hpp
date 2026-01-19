#pragma once
#include "bits.hpp"
#include "opcodes.hpp"
#include <assert.h>
#include <bitset>
#include <cstdint>
#include <iostream>

struct Decode {
  // This class decodes the instructions

  Decode() = delete;
  // First we check the top level decode bits
  static Type_Index decode(uint32_t inst) {
    Type_Index ret;
    switch ((inst & 0b1111111)) {
    case OPCODES::OP_R:
      ret.type = ITYPES::R;
      ret.rindex = decodeR(inst);
      break;
    case OPCODES::OP_IMM_1:
      ret.type = ITYPES::I;
      ret.iindex = decodeIMath(inst);
      break;
    case OPCODES::OP_IMM_2:
      ret.type = ITYPES::I;
      ret.iindex = decodeILoad(inst);
      break;
    case OPCODES::OP_IMM_3:
      ret.type = ITYPES::I;
      ret.iindex = decodeIJump(inst);
      break;
    case OPCODES::OP_IMM_4:
      ret.type = ITYPES::I;
      ret.iindex = decodeICall(inst);
      break;
    case OPCODES::OP_J:
      ret.type = ITYPES::J;
      ret.jindex = JFuncIndex::JAL;
      break;
    case OPCODES::OP_S:
      ret.type = ITYPES::S;
      ret.sindex = decodeS(inst);
      break;
    case OPCODES::OP_B:
      ret.type = ITYPES::B;
      ret.bindex = decodeB(inst);
      break;
    case OPCODES::OP_ULUI:
      ret.type = ITYPES::U;
      ret.uindex = UFuncIndex::LUI;
      break;
    case OPCODES::OP_UAUIPC:
      ret.type = ITYPES::U;
      ret.uindex = UFuncIndex::AUIPC;
      break;
    default:
      std::cout << "Cannot decode the instruction: " << std::bitset<32>{inst}
                << "\n";
      exit(1);
    }
    return ret;
  }
  // TODO: Fill these in correctly to complete the decoder.
  constexpr static RFuncIndex decodeR(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    uint8_t func7 = inst >> INST_BIT_SHIFT::FUNCT7_SHIFT & Masks::FUNC7_MASK;
    // Now the mux for decoding the instruction
    if (func3 == RI_FUNC3::OP_F3_ADD && func7 == R_FUNC7::OP_F7_ADD) {
      return RFuncIndex::ADD;
    } else if (func3 == RI_FUNC3::OP_F3_SUB && func7 == R_FUNC7::OP_F7_SUB) {
      return RFuncIndex::SUB;
    } else if (func3 == RI_FUNC3::OP_F3_XOR && func7 == R_FUNC7::OP_F7_XOR) {
      return RFuncIndex::XOR;
    } else if (func3 == RI_FUNC3::OP_F3_OR && func7 == R_FUNC7::OP_F7_OR) {
      return RFuncIndex::OR;
    } else if (func3 == RI_FUNC3::OP_F3_AND && func7 == R_FUNC7::OP_F7_AND) {
      return RFuncIndex::AND;
    } else if (func3 == RI_FUNC3::OP_F3_SLL && func7 == R_FUNC7::OP_F7_SLL) {
      return RFuncIndex::SLL;
    } else if (func3 == RI_FUNC3::OP_F3_SRL && func7 == R_FUNC7::OP_F7_SRL) {
      return RFuncIndex::SRL;
    } else if (func3 == RI_FUNC3::OP_F3_SRA && func7 == R_FUNC7::OP_F7_SRA) {
      return RFuncIndex::SRA;
    } else if (func3 == RI_FUNC3::OP_F3_SLT && func7 == R_FUNC7::OP_F7_SLT) {
      return RFuncIndex::SLT;
    } else if (func3 == RI_FUNC3::OP_F3_SLTU && func7 == R_FUNC7::OP_F7_SLTU) {
      return RFuncIndex::SLTU;
    } else if (func3 == R_FUNC3::OP_F3_MUL && func7 == R_FUNC7::OP_F7_MUL_DIV) {
      return RFuncIndex::MUL;
    } else if (func3 == R_FUNC3::OP_F3_MULH &&
               func7 == R_FUNC7::OP_F7_MUL_DIV) {
      return RFuncIndex::MULH;
    } else if (func3 == R_FUNC3::OP_F3_MULSU &&
               func7 == R_FUNC7::OP_F7_MUL_DIV) {
      return RFuncIndex::MULSU;
    } else if (func3 == R_FUNC3::OP_F3_MULU &&
               func7 == R_FUNC7::OP_F7_MUL_DIV) {
      return RFuncIndex::MULU;
    } else if (func3 == R_FUNC3::OP_F3_DIV && func7 == R_FUNC7::OP_F7_MUL_DIV) {
      return RFuncIndex::DIV;
    } else if (func3 == R_FUNC3::OP_F3_DIVU &&
               func7 == R_FUNC7::OP_F7_MUL_DIV) {
      return RFuncIndex::DIVU;
    } else if (func3 == R_FUNC3::OP_F3_REM && func7 == R_FUNC7::OP_F7_MUL_DIV) {
      return RFuncIndex::REM;
    } else if (func3 == R_FUNC3::OP_F3_REMU &&
               func7 == R_FUNC7::OP_F7_MUL_DIV) {
      return RFuncIndex::REMU;
    } else {
      std::cout << "Cannot decode R-type instruction: " << std::bitset<32>{inst}
                << "\n";
      exit(1);
    }
  }
  constexpr static BFuncIndex decodeB(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    switch (func3) {
    case B_FUNC3::OP_F3_BEQ:
      return BFuncIndex::BEQ;
    case B_FUNC3::OP_F3_BNE:
      return BFuncIndex::BNE;
    case B_FUNC3::OP_F3_BLT:
      return BFuncIndex::BLT;
    case B_FUNC3::OP_F3_BGE:
      return BFuncIndex::BGE;
    case B_FUNC3::OP_F3_BLTU:
      return BFuncIndex::BLTU;
    case B_FUNC3::OP_F3_BGEU:
      return BFuncIndex::BGEU;
    default:
      std::cout << "Cannot decode B-type instruction: " << std::bitset<32>{inst}
                << "\n";
      exit(1);
    }
  }
  static SFuncIndex decodeS(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    switch (func3) {
    case S_FUNC3::OP_S_B:
      return SFuncIndex::SB;
    case S_FUNC3::OP_S_H:
      return SFuncIndex::SH;
    case S_FUNC3::OP_S_W:
#ifdef DEBUG
      std::cout << "Performing STORE WORD\n";
#endif      
      return SFuncIndex::SW;
    default:
      std::cout << "Cannot decode S-type instruction: " << std::bitset<32>{inst}
                << "\n";
      exit(1);
    }
  }
  static IFuncIndex decodeILoad(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    switch (func3) {
    case ILOAD_FUNC3::OP_F3_B:
      return IFuncIndex::LB;
    case ILOAD_FUNC3::OP_F3_H:
      return IFuncIndex::LH;
    case ILOAD_FUNC3::OP_F3_W:
      return IFuncIndex::LW;
    case ILOAD_FUNC3::OP_F3_BU:
      return IFuncIndex::LBU;
    case ILOAD_FUNC3::OP_F3_HU:
      return IFuncIndex::LHU;
    default:
      std::cout << "Cannot decode I-type instruction: " << std::bitset<32>{inst}
                << "\n";
      exit(1);
    }
  }
  static IFuncIndex decodeIJump(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    if (func3 == IJ_FUNC3::OP_F3_JALR)
      return IFuncIndex::JALR;
    else {
      std::cout << "Cannot decode I-type instruction: " << std::bitset<32>{inst}
                << "\n";
      exit(1);
    }
  }
  static IFuncIndex decodeIMath(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    // XXX: Check that we don't need to sign extend the immediate value.
    word_t imm = (inst >> INST_BIT_SHIFT::I_IMM_SHIFT) & Masks::I_IMM_MASK;

    if (func3 == RI_FUNC3::OP_F3_ADD)
      return IFuncIndex::ADD;
    else if (func3 == RI_FUNC3::OP_F3_XOR)
      return IFuncIndex::XOR;
    else if (func3 == RI_FUNC3::OP_F3_OR)
      return IFuncIndex::OR;
    else if (func3 == RI_FUNC3::OP_F3_AND)
      return IFuncIndex::AND;
    else if (func3 == RI_FUNC3::OP_F3_SLL && (imm >> 5) == 0x00)
      // FIXME: This need to check the immediate value part too.
      return IFuncIndex::SLLI;
    else if (func3 == RI_FUNC3::OP_F3_SRL && (imm >> 5) == 0x00)
      // FIXME: This need to check the immediate value part too.
      return IFuncIndex::SRLI;
    else if (func3 == RI_FUNC3::OP_F3_SRA && (imm >> 5) == 0x20)
      // FIXME: This need to check the immediate value part too.
      return IFuncIndex::SRAI;
    else if (func3 == RI_FUNC3::OP_F3_SLT)
      return IFuncIndex::SLTI;
    else if (func3 == RI_FUNC3::OP_F3_SLTU)
      return IFuncIndex::SLTIU;
    else {
      std::cout << "Cannot decode I-type instruction: " << std::bitset<32>{inst}
                << "\n";
      exit(1);
    }
  }
  static IFuncIndex decodeICall(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    // XXX: Check that we don't need to sign extend the immediate value.
    word_t imm = (inst >> INST_BIT_SHIFT::I_IMM_SHIFT) & Masks::I_IMM_MASK;
    if (func3 == IE_FUNC3::OP_F3_ECALL && imm == 0x0)
      return IFuncIndex::ECALL;
    else if (func3 == IE_FUNC3::OP_F3_EBREAK && imm == 0x1) {
      std::cout << "No support for EBREAK yet!\n";
      assert(false);
    } else {
      std::cout << "Cannot decode I-type instruction: " << std::bitset<32>{inst}
                << "\n";
      exit(1);
    }
  }
};
