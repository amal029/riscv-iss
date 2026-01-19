#define SYS_WRITE 64
#define SYS_READ 63
#define SYS_EXIT 94

void write(int fd, char *buf, unsigned int length);

void _start() {
  char *msg = "Hello, World";
  write(1, msg, 13);
  while (1)
    ;
}

// Function to print a string using RISC-V ecall
void write(int fd, char *buf, unsigned int len) {
  register int a0 asm("a0") = fd;           /* file descriptor to write to */
  register char *a1 asm("a1") = buf;        /* address */
  register unsigned int a2 asm("a2") = len; /* length of the string */
  register long a7 asm("a7") = SYS_WRITE;   // SYS_write

  asm volatile("ecall"
               : /* no output */
               : "r"(a0), "r"(a1), "r"(a2), "r"(a7));
}
