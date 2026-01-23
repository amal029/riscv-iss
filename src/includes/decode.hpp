#pragma once
#include "bits.hpp"
#include "execute.hpp"
#include "opcodes.hpp"
#include <assert.h>
#include <bitset>
#include <cstdint>
#include <iostream>

struct Decode {

  // Error output
  static void error(uint32_t inst) {
    std::cerr << "Cannot decode the instruction: " << std::bitset<32>{inst}
              << "\n";
    std::cerr << "Termination occurred at " << __FILE__ << ":" << __LINE__
              << std::endl;
  }

  // Custom ctor
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
    case Float_OPCODES::Float_I:
      ret.type = ITYPES::FloatI;
      ret.fiindex = decodeFloatI(inst);
      break;
    case Float_OPCODES::Float_S:
      ret.type = ITYPES::FloatS;
      ret.fsindex = decodeFloatS(inst);
      break;
    case Float_OPCODES::Float_R4_ADD:
      ret.type = ITYPES::FloatR4;
      ret.fr4index = Float_R4Index::FMADD_S;
      break;
    case Float_OPCODES::Float_R4_SUB:
      ret.type = ITYPES::FloatR4;
      ret.fr4index = Float_R4Index::FMSUB_S;
      break;
    case Float_OPCODES::Float_R4_NMSUB:
      ret.type = ITYPES::FloatR4;
      ret.fr4index = Float_R4Index::FNMSUB_S;
      break;
    case Float_OPCODES::Float_R4_NMADD:
      ret.type = ITYPES::FloatR4;
      ret.fr4index = Float_R4Index::FNMADD_S;
      break;
    case Float_OPCODES::Float_R:
      ret.type = ITYPES::FloatR;
      ret.frindex = decodeFloatR(inst);
      break;
    default:
      error(inst);
      std::abort();
    }
    return ret;
  }

  constexpr static Float_IIndex decodeFloatI(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    if (func3 == 0b010) {
      return Float_IIndex::FLW;
    } else {
      error(inst);
      std::abort();
    }
  }

  constexpr static Float_RIndex decodeFloatR(uint32_t inst) {
#ifdef DEBUG
    std::cout << std::bitset<32>{inst} << "\n";
#endif
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    uint8_t func7 = inst >> INST_BIT_SHIFT::FUNCT7_SHIFT & Masks::FUNC7_MASK;
    switch (func7) {
    case FExtension_Funct7::FADD_S_Funct7: {
      return Float_RIndex::FADD_S;
      break;
    }
    case FExtension_Funct7::FSUB_S_Funct7: {
      return Float_RIndex::FSUB_S;
      break;
    }
    case FExtension_Funct7::FMUL_S_Funct7: {
      return Float_RIndex::FMUL_S;
      break;
    }
    case FExtension_Funct7::FDIV_S_funct7: {
      return Float_RIndex::FDIV_S;
      break;
    }
    case FExtension_Funct7::FSQRT_S_Funct7: {
      return Float_RIndex::FSQRT_S;
      break;
    }
      // XXX: The below case handles 3 different opcodes
    case FExtension_Funct7::FSGNJ_S_funct7: {
      if (func3 == FExtension_Funct3::FSGNJ_S_Funct3)
        return Float_RIndex::FSGNJ_S;
      else if (func3 == FExtension_Funct3::FSGNJN_S_Funct3)
        return Float_RIndex::FSGNJN_S;
      else if (func3 == FExtension_Funct3::FSGNJX_S_Funct3)
        return Float_RIndex::FSGNJX_S;
      else {
        error(inst);
        std::abort();
      }
      break;
    }
    case FExtension_Funct7::FMIN_S_Funct7: {
      switch (func3) {
      case FExtension_Funct3::FMIN_S_Funct3: {
        return Float_RIndex::FMIN_S;
        break;
      }
      case FExtension_Funct3::FMAX_S_Funct3: {
        return Float_RIndex::FMAX_S;
        break;
      }
      default: {
        error(inst);
        std::abort();
      }
      }
      break;
    }
    case FExtension_Funct7::FCVT_W_S_Funct7: {
#ifdef DEBUG
      std::cout << "FCVT_W_S_Func7 instruction\n";
#endif
      uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
      switch (rs2) {
      case 0x0:
        return Float_RIndex::FCVT_W_S;
      case 0x1:
        return Float_RIndex::FCVT_WU_S;
      default:
        error(inst);
        std::abort();
      }
      break;
    }
    case FExtension_Funct7::FMV_X_W_Funct7: {
      if (func3 == FExtension_Funct3::FMV_X_W_Funct3) {
        uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
        if (rs2 == 0) {
          return Float_RIndex::FMV_X_W;
        } else {
          error(inst);
          std::abort();
        }
      } else if (func3 == FExtension_Funct3::FCLASS_S_Funct3) {
        uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
        if (rs2 == 0b001) {
          return Float_RIndex::FCLASS_S;
        } else {
          error(inst);
          std::abort();
        }
      } else {
        error(inst);
        std::abort();
      }
      break;
    }
    case FExtension_Funct7::FEQ_S_Funct7: {
      switch (func3) {
      case FExtension_Funct3::FEQ_S_Funct3:
        return Float_RIndex::FEQ_S;
      case FExtension_Funct3::FLT_S_Funct3:
        return Float_RIndex::FLT_S;
      case FExtension_Funct3::FLE_S_Funct3:
        return Float_RIndex::FLE_S;
      default:
        error(inst);
        std::abort();
      }
    }
    case FExtension_Funct7::FCVT_S_W_Funct7: {
      uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
      if (rs2 == 0) {
        return Float_RIndex::FCVT_S_W;
      } else if (rs2 == 0b00001) {
        return Float_RIndex::FCVT_S_WU;
      } else {
        error(inst);
        std::abort();
      }
    }
    case FExtension_Funct7::FMV_W_X_Funct7: {
      uint8_t rs2 = ((inst >> INST_BIT_SHIFT::RS2_SHIFT) & Masks::RS2_MASK);
      if (func3 == FExtension_Funct3::FMV_W_X_Funct3 && rs2 == 0) {
        return Float_RIndex::FMV_W_X;
      } else {
        error(inst);
        std::abort();
      }
    }
    default: {
      error(inst);
      std::abort();
    }
    }
  }

  constexpr static Float_SIndex decodeFloatS(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    if (func3 == 0b010) {
      return Float_SIndex::FSW;
    } else {
      error(inst);
      std::abort();
    }
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
      return RFuncIndex::MULHU;
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
      error(inst);
      std::abort();
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
      error(inst);
      std::abort();
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
      error(inst);
      std::abort();
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
      error(inst);
      std::abort();
    }
  }
  static IFuncIndex decodeIJump(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    if (func3 == IJ_FUNC3::OP_F3_JALR)
      return IFuncIndex::JALR;
    else {
      error(inst);
      std::abort();
    }
  }
  static IFuncIndex decodeIMath(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    // XXX: Check that we don't need to sign extend the immediate value.
    word_t imm = (inst >> INST_BIT_SHIFT::I_IMM_SHIFT) & Masks::I_IMM_MASK;

    if (func3 == RI_FUNC3::OP_F3_ADD)
      return IFuncIndex::ADDI;
    else if (func3 == RI_FUNC3::OP_F3_XOR)
      return IFuncIndex::XORI;
    else if (func3 == RI_FUNC3::OP_F3_OR)
      return IFuncIndex::ORI;
    else if (func3 == RI_FUNC3::OP_F3_AND)
      return IFuncIndex::ANDI;
    else if (func3 == RI_FUNC3::OP_F3_SLL && (imm >> 5) == 0x00)
      return IFuncIndex::SLLI;
    else if (func3 == RI_FUNC3::OP_F3_SRL && (imm >> 5) == 0x00)
      return IFuncIndex::SRLI;
    else if (func3 == RI_FUNC3::OP_F3_SRA && (imm >> 5) == 0x20)
      return IFuncIndex::SRAI;
    else if (func3 == RI_FUNC3::OP_F3_SLT)
      return IFuncIndex::SLTI;
    else if (func3 == RI_FUNC3::OP_F3_SLTU)
      return IFuncIndex::SLTIU;
    else {
      error(inst);
      std::abort();
    }
  }
  static IFuncIndex decodeICall(uint32_t inst) {
    uint8_t func3 = inst >> INST_BIT_SHIFT::FUNCT3_SHIFT & Masks::FUNC3_MASK;
    // XXX: Check that we don't need to sign extend the immediate value.
    word_t imm = (inst >> INST_BIT_SHIFT::I_IMM_SHIFT) & Masks::I_IMM_MASK;
    if (func3 == IE_FUNC3::OP_F3_ECALL && imm == 0x0)
      return IFuncIndex::ECALL;
    else if (func3 == IE_FUNC3::OP_F3_EBREAK && imm == 0x1) {
      error(inst);
      std::abort();
    } else {
      error(inst);
      std::abort();
    }
  }
};
