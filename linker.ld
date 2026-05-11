/*Entry point after reset*/
ENTRY(Reset_Handler)

/*Memory layout*/
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM  (rwx): ORIGIN = 0x20000000, LENGTH = 128K
}

/*Initial Stack Pointer*/
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

/*Heap and Stack size declaration*/
_max_heap_size = 0x200;
_max_stack_size = 0x400;

/*Sections across the memory*/
SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        *(.isr_vector_table)
        *(.text)
        *(.rodata)
        . = ALIGN(4);
        _etext = .;
    }>FLASH

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    }>SRAM AT>FLASH 

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        . = ALIGN(4);
        _ebss = .;
    }>SRAM
}   

