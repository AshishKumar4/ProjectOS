#include "stdio.h"
#include "stdarg.h"
#include "string.h"

// Misc functions
// itoa(), Convert an integer into a string
void itoa(int n, char *s, char base)
{
		unsigned long i, j;
		unsigned char temp[20], digit;

		i = 0;
		do
		{
			j = n;
			n /= base; // Divide num by base 
			n *= base; // Multiply num by b to isolate the digit 
			digit = j - n;
		
			// Change the numerical digit to printable ASCII value 
			if(digit <= 9)
			{
				temp[i] = digit + '0';
			}
			else
			{
				temp[i] = digit - 0x0A + 'A';
			}
				n /= base; // Divide num by b again to get to the next digit 
				i++;
		} 
			
		while(n != 0);
	
		temp[i] = '\0';	// Add the terminator 

		// Reserve the string 
		j = strlen(temp) - 1;
		for(i=0; i<strlen(temp); i++)
		{
			s[i] = temp[j];
			j--; // Decrement j while incrementing i 
		}
    	s[i] = '\0'; // Terminate the string 
}

// String functions
// strlen(), Returns length of a string 
size_t strlen(char *s)
{
		size_t i=0;
		while (*(s++) != '\0')
		{	
			i++;
		}
		return i;
}

// strncmp(), Compares first n characters of two strings 
int strncmp(const char *s1, const char *s2, size_t n)
{
		int i = 1;

		for (; *s1 == *s2 && i <= n; s1++, s2++, i++)
		if (i == n)
		{
			return 0; // Equal
		}	 
		return *s1 - *s2;
}

// strcmp(), Compares two strings 
int strcmp(const char *s1, const char *s2)
{
		for (; *s1 == *s2; s1++, s2++)
		if (*s1 == '\0')
		{	 
			return 0; // Equal
		}  
		return *s1 - *s2;
}

// strcat(), Adds src to dest 
char *strcat(char *dest, const char *src)
{
		for (; *dest != '\0'; dest++)
		; // Go through dest until we reach a NUL 

	    while (*src != '\0')
        {
			*(dest++) = *(src++);
			*dest = '\0';
		}
		return dest;
}

// strncat(), Adds n characters of src to dest 
char *strncat(char *dest, const char *src, size_t n)
{
	
		for (; *dest != '\0'; dest++)
		;	
		while (*src != '\0' && n-- > 0)
		{
			*(dest++) = *(src++);
			*dest = '\0';
		}
		return dest;
}

// strcpy(). Copies src to dest 
char *strcpy(char *dest, const char *src)
{
		const char *s = src;
		char *d = dest;

		while (*s != '\0')
		{
			*(d++) = *(s++);
		}
		return dest;
}

// sprintf(). Prints format to str 
int sprintf(char *str, const char *format, ...)
{
		va_list ap;

		va_start(ap, format);
		vsprintf(str, format, ap);
		va_end(ap);

		return strlen(str);
}

// Memory functions
// memcpy(). Copies n bytes from src to dest
void *memcpy(void *dest, const void *src, size_t n)
{
		char *to = (char*) dest, *from = (char*) src;

		while (n-- > 0)
        {
			*(to++) = *(from++);
		}
		return dest;
}

// memset(). Fills the first n bytes of s with the constant
// byte c
void *memset(void *s, int c, size_t n)
{
		char *to = (char*) s;

		while (n-- > 0)
		{	 
			*(to++) = c;
        }
		return s;
}
