#define SYS_WRITE 64
#define SYS_READ 63		/* TODO */
#define SYS_EXIT 94


// Function to print a buffer using RISC-V ecall
void write(int fd, void *buf, unsigned int len) {
  register int a0 asm("a0") = fd;           /* file descriptor to write to */
  register void *a1 asm("a1") = buf;        /* address */
  register unsigned int a2 asm("a2") = len; /* length of the buffer */
  register long a7 asm("a7") = SYS_WRITE;   /* SYS_WRITE */

  asm volatile("ecall"
               : /* no output */
               : "r"(a0), "r"(a1), "r"(a2), "r"(a7));
}
