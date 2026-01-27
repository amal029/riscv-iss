#pragma once

#include "../includes/system.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

enum class TAG : std::int8_t { BASE_REG = 0, FLOAT_REG = 1, MEM = 2, PC = -1 };

// This holds all the record for reverse debugging
struct Record {
  size_t PC;    // The program counter
  size_t index; // The index to save the word in
  TAG tag;
  uint8_t word[WORD]; // The, usually 4-byte, word to save
};

// This object should be passed to the execute unit to save things
// before executing the instruction.
struct Log {

  // ctor
  Log(word_t *reg, fword_t *freg, uint8_t *mem)
      : REGFILE(reg), MEM(mem), FREGFILE(freg) {};

  static constexpr const char *tag2Type(const TAG tag) noexcept {
    switch (tag) {
    case TAG::BASE_REG:
      return "BASE_REG";
    case TAG::FLOAT_REG:
      return "FLOAT_REG";
    case TAG::MEM:
      return "MEMORY";
    case TAG::PC:
      return "PC";
    default:
      return "UNKNOWN";
    }
  }

  void print_record(const Record &x) const {
    printf("Record{\t");
    printf("PC: %lx, index: %lx, TAG: %s, word: [", x.PC, x.index,
           tag2Type(x.tag));
    for (auto i = 0; i < WORD; ++i) {
      printf("%02x", x.word[i]);
      if (i == WORD - 1)
        printf("]");
    }
    std::cout << "}\n";
  }

  void print_log() const {
    for (const Record &x : rec) {
      print_record(x);
    }
  }

  void recordFRegWord(size_t PC, uword_t freg_index) {
    Record rr{PC, freg_index, TAG::FLOAT_REG, {0x0}};
    std::memcpy(rr.word, FREGFILE + freg_index, WORD);
    rec.push_back(rr);
  }
  void recordRegWord(size_t PC, uword_t reg_index) {
    Record rr{PC, reg_index, TAG::BASE_REG, {0x0}};
    std::memcpy(rr.word, REGFILE + reg_index, WORD);
    rec.push_back(rr);
  }
  void recordMem(size_t PC, uword_t mem_add) {
    // Get the whole mem word that needs to be saved
    mem_add = mem_add - (mem_add % WORD); // Get the start of the word
    Record rr{PC, mem_add, TAG::MEM, {0x0}};
    std::memcpy(rr.word, MEM + mem_add, WORD);
    rec.push_back(rr);
  }
  void recordPC(size_t PC) { rec.push_back({PC, 0, TAG::PC, {0x0}}); }
  // We can record the values everytime
  void record(size_t PC, uword_t *mem_add = nullptr,
              uword_t *reg_index = nullptr, uword_t *freg_index = nullptr) {
    if (mem_add != nullptr) {
      // Here we want to store the mem word into the record
      recordMem(PC, *mem_add);
    } else if (reg_index != nullptr) {
      recordRegWord(PC, *reg_index);
    } else if (freg_index != nullptr) {
      recordFRegWord(PC, *freg_index);
    } else {
      recordPC(PC);
    }
  }

  // Getting the recorded values out and resetting the values in the
  // correct positions.
  void reverse(size_t *cPC) {
    if (rec.size() == 0)
      return;
    Record &rr = rec.back();
    *cPC = rr.PC; // set the PC back
    switch (rr.tag) {
    case TAG::BASE_REG:
      // Restore register value
      std::memcpy(REGFILE + rr.index, rr.word, WORD);
      break;
    case TAG::FLOAT_REG:
      // Restore floating point register value
      std::memcpy(FREGFILE + rr.index, rr.word, WORD);
      break;
    case TAG::MEM:
      // Restore memory word
      std::memcpy(MEM + rr.index, rr.word, WORD);
      break;
    case TAG::PC:
      // Do nothing
      break;
    }
    rec.pop_back();
  }

private:
  word_t *REGFILE;
  uint8_t *MEM;
  fword_t *FREGFILE;
  std::vector<Record> rec; // AoS -- OK, because no throughput needed.
};
