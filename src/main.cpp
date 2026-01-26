#include "./includes/bits.hpp"
#include "./includes/decode.hpp"
#include "./includes/execute.hpp"
#include "./includes/fetch.hpp"
#include "./includes/system.hpp"
#include "./debug/debug.hpp"
#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

#define IS_LITTLE_ENDIAN ((*(const char *)&(const int){1}) == 1)

struct Config {
  constexpr static size_t KB = 1024;
};

void print_word(word_t x) {
  std::cout << std::hex << std::setfill('0') << std::setw(8)
            << static_cast<int>(x);
}

void print_byte(uint8_t x) {
  std::cout << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(x);
}

void print_fregisters(fword_t *regs) {
  for (size_t i = 0; i < REGS; ++i) {
    if (i <= 7) {
      std::cout << "ft" << i << ": " << (float)regs[i];
    } else if (i >= 8 && i <= 9) {
      std::cout << "fs" << (i - 8) << ": " << (float)regs[i];
    } else if (i >= 10 && i <= 17) {
      std::cout << "fa" << (i - 10) << ": " << (float)regs[i];
    } else if (i >= 18 && i <= 27) {
      std::cout << "fs" << (i - 18 + 2) << ": " << (float)regs[i];
    } else {
      std::cout << "ft" << (i - 28) << ": " << (float)regs[i];
    }
    if (i > 0 && i % 4 == 0)
      std::cout << "\n";
    else
      std::cout << "\t";
  }
  std::cout << "\n";
}

void print_registers(word_t *regs, size_t PC) {
  for (size_t i = 0; i < REGS; ++i) {
    if (i == 2) {
      std::cout << "sp: ";
    } else if (i >= 5 && i <= 7) {
      std::cout << "t" << (i - 5) << ": ";
    } else if (i >= 28 && i <= 31) {
      std::cout << "t" << (i - 25) << ": ";
    } else if (i == 1)
      std::cout << "ra: ";
    else if (i == 3)
      std::cout << "gp: ";
    else if (i == 8) {
      std::cout << "s0: ";
    } else if (i >= 10 && i <= 17) {
      std::cout << "a" << (i - 10) << ": ";
    } else if (i >= 18 && i <= 27) {
      std::cout << "s" << (i - 16) << ": ";
    } else {
      std::cout << "x" << i << ": ";
    }
    print_word(regs[i]);
    if (i > 1 && i % 4 == 0)
      std::cout << "\n";
    else
      std::cout << "\t";
  }
  std::cout << "PC = ";
  print_word(PC);
  std::cout << "\n";
}

void print_mem(uint8_t *MEM) {
  for (size_t i = 0; i < (ISIZE + DSIZE) * Config::KB; ++i) {
    if (i % 4 == 0)
      std::cout << "\n";
    else
      std::cout << " ";

    if (i % 4 == 0)
      printf("%4lu:", i);
    print_byte(MEM[i]);
  }
  std::cout << "\n";
}

// Load the binary File
std::vector<uint8_t> readFile(const std::string &filename) {
  auto size = std::filesystem::file_size(filename);
  std::vector<uint8_t> buffer(size);
  std::ifstream file(filename, std::ios::binary);
  file.read((char *)buffer.data(), size);
  return buffer;
}

void execute(uint32_t inst, uint8_t *MEM, size_t &PC, Execute &exec, bool br) {
  exec.setPC_Change();
  // First fetch the instruction from I-memory
  inst = Fetch::fetch(MEM, PC);
  if (br) {
    fprintf(stderr, "%zx:", PC);
    print_word(inst);
    std::cout << "\n";
  }

  // Then Decode instruction using decode class
  Type_Index val = Decode::decode(inst);
  // Then execute the instruction using the Execute class
  exec.execute(val, inst);
  // If the PC has not changed then we increment the PC by 4 bytes (next
  // instruction)
  if (!exec.getPC_Change())
    PC = PC + 4;
}

void add_break_points(std::vector<size_t> &breaks) {
  std::cout << "Add breakpoints as numbers in hex, e.g., fc\n";
  std::cout << "q for quitting\n";
  std::string in;
  while (1) {
    std::cin >> in;
    if (in == "q") {
      break;
    } else {
      char *endptr;
      size_t result;
      errno = 0;
      result = strtoul(in.c_str(), &endptr, 16);
      if (result == 0 && in.c_str() == endptr) {
        std::cerr << "Cannot parse input, try again\n";
        std::cerr << "Add breakpoints as numbers in hex, e.g., fc\n";
        std::cerr << "q for quitting\n";
      } else if (errno == ERANGE) {
        printf("Input value out of range\n");
      } else {
        breaks.emplace_back(result);
        std::cout << "added breakpoint: " << in << "\n";
      }
    }
  }
}

int main(int argc, char **argv) {

#ifndef IS_LITTLE_ENDIAN
  static_assert(false, "Only Little Endian architecture supported");
#endif

  std::vector<uint8_t> file;
  if (argc < 2) {
    std::cout << "Please run the program with ./iss <file-name>.bin\n";
    exit(1);
  } else {
    file = readFile(std::string{argv[1]});
  }
  // The program counter -- starts at the _start
  size_t PC = 0; // loads the program into location 0
  // Flag to see of the PC has changed.
  bool PC_change = false;

  // This is the register file -- 32-bit machine (Ints)
  word_t REGFILE[REGS] = {0};

  // This is the register file -- 32-bit machine (Floats)
  fword_t REGFILE_FLOAT[REGS] = {0.0f};

  // This is the data + instruction memory with shared address space.
  uint8_t MEM[(ISIZE + DSIZE) * Config::KB] = {0x00}; // total mem size

  // Get the text (program) + data into memory
  std::memcpy(MEM, file.data(), file.size());

  uint32_t inst = 0;
  // Debug/Log system
  Log log{REGFILE, REGFILE_FLOAT, MEM};
  
  // Executor
  Execute exec{REGFILE, REGFILE_FLOAT, MEM, &PC, &PC_change, &log};

  
  unsigned char input;

  // Add all the breakpoints
  std::vector<size_t> breaks{};

  std::cout << "p: for printing registers\n";
  std::cout << "m: for printing memory\n";
  std::cout << "a: for adding breakpoints\n";
  std::cout << "l: for printing registered breakpoints\n";
  std::cout << "r: to run the program\n";
  std::cout << "s: to step to next instruction\n";
  std::cout << "z: for printing the recorded trace\n";
  std::cout << "u: for going 1 step backwards (reverse-debugging)\n";
  std::cout << "q: to quit\n";

  std::cin >> input;

  while (1) {
    // Here we can dump the registers and memory if asked for.
    if (input == 'a') {
      add_break_points(breaks);
      std::cin >> input;
    } else if (input == 'p') {
      print_registers(REGFILE, PC);
      print_fregisters(REGFILE_FLOAT);
      std::cin >> input;
    } else if (input == 'm') {
      print_mem(MEM);
      std::cin >> input;
    } else if (input == 's') {
      execute(inst, MEM, PC, exec, (!breaks.empty()));
      std::cin >> input;
    } else if (input == 'r') {
      execute(inst, MEM, PC, exec, (!breaks.empty()));
      // Check if the PC has reached one of the break points then stop
      // and take input.
      if (std::find(breaks.cbegin(), breaks.cend(), PC) != breaks.cend())
        std::cin >> input;
    } else if (input == 'l') {
      // Print all the registered breakpoints
      for (size_t br : breaks) {
        std::cout << std::hex << std::setfill('0') << std::setw(8) << br
                  << ", \t";
      }
      std::cout << "\n";
      std::cin >> input;
    } else if (input == 'q') {
      break;
    } else if (input == 'z') {
      log.print_log();
      std::cin >> input;
    } else if (input == 'u') {
      log.reverse(&PC);
      std::cin >> input;
    } else {
      std::cin >> input;
    }
  }
  return 0;
}
