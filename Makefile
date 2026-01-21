CXXFLAGS := -O3 -std=c++17 -Wall -Werror -Wextra -Wno-unused-parameter -Wshadow
CXX ?= g++-m			# if not already given

RISCVCC  := riscv64-unknown-elf-gcc
RISCVOBJ := riscv64-unknown-elf-objcopy

RISCV_ARCH_FLAGS := -march=rv32imf -mabi=ilp32f -ffreestanding -nostdlib \
		-ffunction-sections -Wl,--gc-sections
RISCV_COMMON_FLAGS := $(RISCV_ARCH_FLAGS) -Wall -Wextra -Werror

# The flags for com
RISCVFLAGS := -O3 $(RISCV_COMMON_FLAGS)

# The flags for compiling our libc
RISCVLIBFLAGS := -O3 $(RISCV_COMMON_FLAGS)

ASM  := ./lib/asm
LIBC := ./lib/src

CFILES := $(shell find $(LIBC) -name "*.c" -type f)
OBJS   := $(CFILES:.c=.o)

main:
	$(CXX) $(CXXFLAGS) ./src/main.cpp -o iss

clean:
	rm -rf iss ./tests/*.elf ./tests/*.bin $(ASM)/*.o $(LIBC)/*.o

binary:	libc compile_start compile_binary
	$(RISCVOBJ) -O binary $(FNAME).elf $(FNAME).bin

libc:	$(OBJS)

%.o:	%.c
	$(RISCVCC) $(RISCVLIBFLAGS)  -c $< -o $@

compile_start:
	$(RISCVCC) $(RISCVFLAGS) -c $(ASM)/start.S -o $(ASM)/start.o

compile_binary:
	$(RISCVCC) $(RISCVFLAGS) -T linker.ld $(ASM)/start.o \
	$(FNAME).c -o $(FNAME).elf -L$(LIBC) $(OBJS)
