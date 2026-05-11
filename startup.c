#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

int main(void);
void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));

uint32_t vector_table[] __attribute__((section(".isr_vector_table")))={
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
};

void Default_Handler(void){
    while(1){
        //Infinite loop. Currently no handling mechanism
        //Could be modified based on your project or application
        //Same for other weak handlers
        //You could overwrite depending on your requirement
    }
}

void Reset_Handler(void){
    uint32_t data_mem_size=(uint32_t)&_edata - (uint32_t)&_edata;

    uint32_t *flashDataStart=(uint32_t *)&_edata;
    uint32_t *sramDataStart=(uint32_t *)&_sdata;

    for(uint32_t i=0;i<data_mem_size;i++){
        *sramDataStart++ = *flashDataStart++;
    }

    uint32_t bss_mem_size=(uint32_t)&_ebss - (uint32_t)&_ebss;

    uint32_t *srambssStart=(uint32_t *)_sbss;

    for(uint32_t i=0;i<bss_mem_size;i++){
        *srambssStart++ =0;
    }

    main();
}