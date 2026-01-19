#include <cstdint>

// Information about the register file
#define REGS 32 // number of registers

// Information about the size of the instruction memory
#define ISIZE 1 // instruction memory array size in Kilo bytes

// Information about the data memory
#define DSIZE 1 // data memory array size in Kilo bytes

// The word size for the architecture
#define WORD 4 // 4 bytes is 1 word (32-bit architecture)

// The type of the register file
using word_t = int32_t;

// The unsigned version of the word type
using uword_t = uint32_t;
