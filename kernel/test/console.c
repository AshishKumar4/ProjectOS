#include "io.h"
#include "stdio.h"

static unsigned short *_vga_fb_adr;
static unsigned _crtc_io_adr;

// Initialize video
void init_console(void)
{
	    // Clear the screen
	    clear_screen();
        printk("Booting...\n\n"); 
		
		// Check for monochrome or color VGA emulation 
		if((inportb(0x3CC) & 0x01) != 0)
		{
			// Monochrome emulation
			_vga_fb_adr = (unsigned short *)0xB8000L;
			_crtc_io_adr = 0x3D4;
		}
		else
		{
			// Color VGA emulation
			_vga_fb_adr = (unsigned short *)0xB0000L;
			_crtc_io_adr = 0x3B4;
		}

		printk("init_video: %s Emulation\n", (_crtc_io_adr == 0x3D4) ? "Color" : "Monochrome", _vga_fb_adr);
}

