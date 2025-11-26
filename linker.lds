/* Tell the linker that we want an x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)

/* Kmain is our kernel entry point, if mismatch occurs, will crash */
ENTRY(kmain)

/* Define the program headers we want so the bootloader gives us the right */
/* MMU permissions; this also allows us to exert more control over linking proccess */
PHDRS
{
    limine_requests PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
}

SECTIONS
{
    /* We want to be placed in the topmost 2Gib of the address space */
    /* for optimizations and because the Limine spec mandates it */
    /* Any address in the region will do but 0x...00 is the begginning of region */
    . = 0xffffffff80000000;

    /* Contains the Limine requests and assigns it to its own PHDR */
    /* Do not remove KEEP statements or the requests may be deleted by garb collec */
    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    /* Move to the next memory page for .text (code) */
    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .text : {
        *(.text .text.*)
    } :text

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    /* Move to the next memory page for .rodata (read only data: consts) */
    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    /* gnu build output section in case a build ID flag is added to linker cmd */
    .note.gnu.build-id : {
        *(.note.gnu.build-id)
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    /* Move to the next memory page for .data (global and statics) */
    .data : {
        *(.data .data.*)
    } :data

    /* Keep .bss last or unnecessary zeroes will be written to bin */
    /* PUT ALL OTHER DATA ABOVE .BSS */
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    /* Note and eh_frame can causes issues */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}
