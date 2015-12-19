#include "io.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"	 

// vsprintf() - Dissolve conversions 
void vsprintf(char *str, const char *format, va_list ap)
{
		char *p = str;
		char *s; int d; char c;
		int i = 0;
	
		*p = '\0';
		// Go through format until we reach a % 
		while (*format != '\0')
		{
			if (*format != '%')	// Ordinary character 
			{
				i = strlen(p);
				p[i] = *format;
				p[i + 1] = '\0';
			}
		
		else
		{
			switch (*(format + 1))
			{
				case 'c': // Char 
					c = (char) va_arg(ap, int);
					i = strlen(p);
					p[i] = c;
					p[i + 1]  = '\0';
					break;		
				case 's': // String 
					s = va_arg(ap, char *);
					strcat(p, s);
					break;
				case 'd': // Integer 
				case 'i':
					d = va_arg(ap, int);
					itoa(d, s, 10);	// Convert to string 
					strcat(p, s);
					break;
				default:
					// Print the % and the following character 
					i = strlen(p);
					p[i]  = *format;
					p[i + 1]  = *(format + 1);
					p[i + 2]  = '\0';
					break;
			}
			format++;
		}
		format++;
		}
}				
