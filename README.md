# Bare-Metal Driver Development (STM32F446xx)

This repository contains a collection of bare-metal peripheral drivers and low-level experiments developed 
for the STM32F446xx series MCU using the ARM GNU Toolchain, CMSIS device headers, custom linker scripts, and OpenOCD-based flashing/debugging.

The goal of this project was to understand embedded systems from the ground up by working directly with:

-ARM Cortex-M4 architecture
-STM32 reference manuals and datasheets
-Memory mapping and linker scripts
-Startup code and vector tables
-Peripheral register-level programming
-Compilation, linking, ELF generation, and flashing workflow

**No HAL or high-level abstraction libraries were used for the driver implementations.**

---

## What's in the repo:
-Bare-Metal Fundamentals
-Cortex-M4 architecture basics
-Startup file development
-Interrupt vector table setup
-Linker script customization
-Memory section placement (.text, .data, .bss, custom sections)
-Flash vs SRAM initialization flow
-Stack pointer initialization
-NVIC interrupt configuration
-CMSIS integration

---

## Peripheral Drivers Implemented:

### GPIO
Pin mode configuration
Push-pull / open-drain modes
Pull-up / pull-down configuration
Alternate function configuration
Input/output handling

### UART / USART
UART initialization
Transmission and reception
Polling-based communication
DMA-based UART transfers
Interrupt concepts

### SPI
Master mode configuration
Clock polarity and phase configuration
Full-duplex communication basics

### I2C
Peripheral initialization
Clock configuration
CCR/TRISE understanding
Data transmission concepts

### ADC
Single-channel ADC conversion
Sampling time configuration
Resolution selection
DMA-based ADC transfers

## Timers

### SysTick
Delay generation
Periodic timing

### General Purpose Timers
Timer initialization
Counter configuration
Timing/delay concepts

### EXTI
External interrupt configuration
SYSCFG routing
NVIC integration

### RTC
Backup domain configuration
LSE/LSI concepts
BCD conversion handling
RTC initialization flow

### IWDG
Independent watchdog configuration
LSI clock understanding
Timeout/reload concepts

---

## Build and Toolchain Workflow

### The project was built manually using:
-VS Code
-ARM GNU Embedded Toolchain
-OpenOCD
-GDB

### The following concepts were explored practically:

-Compilation into object files (.o)
-Linking using custom linker scripts
-ELF generation
-Flash programming using OpenOCD
-GDB-based flashing/debugging workflow
-Basic understanding of Make-based build automation

### Additional folders/files contain:

object files (.o), experimental projects, ELF outputs ,CMSIS testing examples, simple test applications such as LED blinking.

---

## References

### Primary References
-STM32F446xx Reference Manual
-STM32F446xx Datasheet
-ARM Cortex-M4 User Manual
-CMSIS Documentation

### Learning Resource
**Israel Gbati**’s embedded systems and bare-metal programming material/book.

---

## Note for the the readers

**This repository is primarily a learning-focused bare-metal embedded systems project intended to strengthen my low-level 
understanding of ARM Cortex-M microcontrollers and embedded toolchain internals.**

---

## Author
Jagakishan S.K

