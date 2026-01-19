.global _start
.extern main

_start:
	li sp, 2048 /*Change this in accordance with ISIZE + DSIZE in iss*/
	jal ra, main
	
_j: 	j _j
