#include "8259.h"
#include "io.h"
#include "init_interrupts.h"
#include "timer.h"
#include "stdio.h"

void init_timer(void)
{		
		// Initialize Timer Interrupt
		install_handlers(20, timer_handler, 0);
		enable_irq(20);

		timer_handler(100);
}

void timer_handler(unsigned hz)
{
        unsigned short temp = (3579545L / 3) / hz;
		
		// Re-program the 8253 timer chip to run at 'HZ', instead of 18 Hz 
		outportb(0x43, 0x36);		 // Channel 0, LSB/MSB, mode 3, binary
		outportb(0x40, temp & 0xFF); // LSB
		outportb(0x40, temp >> 8);	 // MSB
}

