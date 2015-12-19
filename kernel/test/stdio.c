#include "io.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

unsigned long print_color = 0x02; 
unsigned char *VIDEO_MEMORY = (char *) 0xB8000;

// clear_screen(), Clears the screen
void clear_screen(void)				
{
	    unsigned int i;

        // Fill the screen with background color 
        for(i = 0; i < (80 * 25); i++)       
        {					
        	VIDEO_MEMORY[i * 2] = 0x20;
        	VIDEO_MEMORY[i * 2 + 1] = 0x07;
        }
        // Set the cursor to the upper-left corner of the screen
        outportb(0x3d4, 0x0f); 
        outportb(0x3d5, 0); 
        outportb(0x3d4, 0x0e); 		
        outportb(0x3d5, 0);
}

// _put_char(), Outputs a character to the screen
int _put_char(unsigned char c)
{
        unsigned int vidmem_off, i;
	
	    // Get cursor Y position	
        outportb(0x3d4, 0x0e); 
        vidmem_off = inportb(0x3d5);
        vidmem_off <<= 8;

        // Add cursor X position	
        outportb(0x3d4, 0x0f); 
        vidmem_off += inportb(0x3d5);
        vidmem_off <<= 1;
	
        switch(c)
        {

		// Newline 
		case '\n':  
                 vidmem_off = (vidmem_off / 160) * 160 + 160;
                 break;
              
        // Carriage return 
        case '\r':  
                 vidmem_off = (vidmem_off / 160) * 160;
                 break;

		// Tabulator
        case '\t': 
                 vidmem_off += 8;
                 break;

		// Delete
        case 8:
                 vidmem_off -= 2;
                 VIDEO_MEMORY[vidmem_off] = 0x20;
                 break;

		// Normal character
        default: 
                 VIDEO_MEMORY[vidmem_off++] = c;
                 VIDEO_MEMORY[vidmem_off++] = 0x07; 
                 break;
        }  

        // Are we off-screen ?
        if(vidmem_off >= 160 * 25) 
        {
            // Scroll the screen up
			for(i = 0; i < 160 * 24; i++) 
            {
                VIDEO_MEMORY[i] = VIDEO_MEMORY[i + 160];
            }         

            // Empty the bottom row
		    for(i = 0; i < 80; i++) 
			{
			    VIDEO_MEMORY[(160 * 24) + (i * 2)] = 0x20;
                VIDEO_MEMORY[(160 * 24) + (i * 2)] = 0x07; 
			}
            vidmem_off -= 160; // Were on the bottom row
        }

        // Set the new cursor position
        vidmem_off >>= 1;   
        outportb(0x3d4, 0x0f);
        outportb(0x3d5, vidmem_off & 0x0ff);
        outportb(0x3d4, 0x0e);
        outportb(0x3d5, vidmem_off >> 8);

        return 1;
}

// putc(), Frontend for _put_char()
int putc(unsigned char c)
{
	    int i;
        print_color = WHITE_BLACK;
        i = _put_char(c);
        return i;
}

// printk(), Prints a message to the screen
int printk(const char *format, ...)
{
	    char buffer[MAXLEN];
        va_list arg;
        int i = 0;

        va_start(arg, format);
        vsprintf(buffer, format, arg);
        va_end(arg);

        print_color = WHITE_BLACK;
        while(buffer[i] != '\0')
            _put_char(buffer[i++]);
        return i;
}

void debug(const char *str, ...)
{
		char buf[MAXLEN];
		va_list arg;
		int i = 0;
	
		va_start(arg, str);
		vsprintf(buf, str, arg);
		va_end(arg);
	
		print_color = 0x02; // Color to use 
		while (buf[i] != '\0')
			_put_char(buf[i++]);
}

