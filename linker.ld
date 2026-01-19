ENTRY(_start)  /* Specifies _start as the entry point */

SECTIONS {
    . = 0x0; /* Example starting address for bare-metal */
    .text : {
        KEEP(*(.text.start)) /* Place _start here, keep it */
        *(.text)
    }
    .data : { *(.data) }
    .bss : { *(.bss) }
}
