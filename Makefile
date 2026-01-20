CXXFLAGS?=-O3 -std=c++17 -Wall -Werror -Wextra -Wno-unused-parameter -Wshadow
CXX=g++-m

RISCVCC=riscv64-unknown-elf-gcc
RISCVOBJ=riscv64-unknown-elf-objcopy
RISCVFLAGS=-O1 -Wall -Wextra -Werror

ASM=./lib/asm

main:
	$(CXX) $(CXXFLAGS) ./src/main.cpp -o iss

clean:
	rm -rf iss ./tests/*.elf ./tests/*.bin $(ASM)/*.o

binary:	compile_start compile_binary
	$(RISCVOBJ) -O binary $(FNAME).elf $(FNAME).bin

# compile_binary:
# 	$(RISCVCC) -nostartfiles \
# 	-Wl,--section-start=.text=0x0 \
# 	-march=rv32im -mabi=ilp32 -ffreestanding -nostdlib \
# 	-Wl,--section-start=.data=0x3ff $(FNAME).c -o $(FNAME).elf

compile_start:
	$(RISCVCC) $(RISCVFLAGS) -march=rv32im -mabi=ilp32 \
	-ffreestanding -nostdlib -c $(ASM)/start.S -o $(ASM)/start.o

compile_binary:
	$(RISCVCC) $(RISCVFLAGS) -T linker.ld -march=rv32im -mabi=ilp32 \
	-ffreestanding -nostdlib $(ASM)/start.o $(FNAME).c -o $(FNAME).elf
