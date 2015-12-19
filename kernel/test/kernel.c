#include "8259.h"
#include "console.h"
#include "init_interrupts.h"
#include "interrupt.h"
#include "io.h"
#include "mm.h"
#include "stdio.h"
#include "string.h"
#include "timer.h"

void kmain(void)
{			
		init_console();
        init_interrupts();
		
		init_timer();
		init_mm();

		// Print some messages to the screen
        printk("\nIBOX Operating System 0.01 [ALPHA] running\n");
		printk("Copyright (C) 2002 by William McCabe <wmccabe@hotmail.com>");

		idle();
}

   
