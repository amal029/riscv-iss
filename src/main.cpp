#include "./includes/bits.hpp"
#include "./includes/decode.hpp"
#include "./includes/execute.hpp"
#include "./includes/fetch.hpp"
#include "./includes/system.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

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

void print_registers(word_t *regs, size_t PC) {
  for (size_t i = 0; i < REGS; ++i) {
    if (i == 2) {
      std::cout << "sp: ";
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

    if (i % 4 == 0) {
      printf("%4lu", i);
      // print_word(i);
      std::cout << ": ";
    }
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

void execute(uint32_t inst, uint8_t *MEM, size_t &PC, Execute &exec) {
  exec.setPC_Change();
  exec.setRegFile(0, 0);
  // First fetch the instruction from I-memory
  inst = Fetch::fetch(MEM, PC);
#ifdef INST  
  fprintf(stderr, "%zx:", PC);
  print_word(inst);
  std::cout << "\n";
#endif
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
  std::cout
      << "Add breakpoints (in hex) like so: \"b <prog-address-in-hex>\"\n";
  std::cout << "q exists adding break points\n";
  std::string input;
  while (std::getline(std::cin, input)) {
    std::cout << input << "\n";
    if (input == "q") {
      break;
    } else {
      std::string token = input.substr(input.find(" "), input.size());
      // Convert from hex to
      breaks.emplace_back(std::stoull(token, nullptr, 16));
    }
  }
}

int main(int argc, char **argv) {
  std::vector<uint8_t> file;
  if (argc < 2) {
    std::cout << "Please run the program with ./iss <file-name>.bin\n";
    exit(1);
  } else {
    std::cout << "Reading the binary file\n";
    file = readFile(std::string{argv[1]});
  }
  // The program counter -- starts at the _start
  size_t PC = 0; // loads the program into location 0
  // Flag to see of the PC has changed.
  bool PC_change = false;

  // This is the register file -- 32-bit machine
  word_t REGFILE[REGS] = {0};

  // This is the data + instruction memory with shared address space.
  uint8_t MEM[(ISIZE + DSIZE) * Config::KB] = {0x00}; // total mem size

  // Get the text (program) + data into memory
  std::memcpy(MEM, file.data(), file.size());

  uint32_t inst = 0;
  // Type_Index val;
  Execute exec{REGFILE, MEM, &PC, &PC_change};
  unsigned char input;

  // Add all the breakpoints
  std::vector<size_t> breaks{};
  add_break_points(breaks);

  // The last byte in memory to allow it to grow upwards.
  // REGFILE[2] = (ISIZE + DSIZE) * Config::KB; // SP
  std::cout << "Press: p for registers, m for memory (shared program then "
            << "data), s to exectute 1 instruction, r for run to program end. "
               "Finally q to quit\n";
  std::cin >> input;
  while (1) {
    // Here we can dump the registers and memory if asked for.
    if (input == 'p') {
      print_registers(REGFILE, PC);
      std::cin >> input;
    } else if (input == 'm') {
      print_mem(MEM);
      std::cin >> input;
    } else if (input == 's') {
      execute(inst, MEM, PC, exec);
      std::cin >> input;
    } else if (input == 'r') {
      execute(inst, MEM, PC, exec);
      // Check if the PC has reached one of the break points then stop
      // and take input.
      if (std::find(breaks.cbegin(), breaks.cend(), PC) != breaks.cend())
        std::cin >> input;
    } else if (input == 'q') {
      break;
    }
  }
  return 0;
}
