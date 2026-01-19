ENTRY(_start)  /* Specifies _start as the entry point */

SECTIONS {
    . = 0x0; /* Starting address for emulator */
    .text : {
        KEEP(*(.text.start)) /* Place _start here, keep it */
        *(.text)	     /* other .text sections */
    }
    .data : { *(.data) }
    .bss : { *(.bss) }
}
