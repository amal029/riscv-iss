#include "../includes/syscodes.h"
.global _start
.extern main

_start:
	li sp, 2048 /*Change this in accordance with ISIZE + DSIZE in iss*/
	jal ra, main
	
_j: 	j _j

.global write

/* This thing assumes that a0 = fd, a1 = *buf, a2 = length of string*/
write:	
	li a7, SYS_WRITE
	ecall
	ret
