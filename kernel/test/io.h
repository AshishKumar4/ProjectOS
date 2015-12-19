#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

// inportb: Get a byte from the selected port
unsigned inportb(unsigned port);

// outportb: Send the selected byte to the selected port
void outportb(unsigned port, unsigned val);

// write_nmi: Write Non Maskable Interrupt
void write_nmi(unsigned char nmi);

// enable_nmi: Enable Non Maskable Interrupt
void enable_nmi(void);

// sti: Enable Interrupts:
void sti();

// cli: Disable Interrupts:
void cli();

// hlt: Halt the CPU:
void hlt();

// idle: Freeze the Computer:
void idle();

// reboot: Reboot the Computer:
void reboot();

#endif

