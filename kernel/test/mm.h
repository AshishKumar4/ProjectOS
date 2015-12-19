#ifndef _INCLUDE_MM_H
#define _INCLUDE_MM_H

void write_cr0();
void write_cr3();

void get_e820Map(void);
void init_paging(void);
void init_mm(void);

// Int 15h E820 Memory Map Entry
typedef struct
{
	unsigned long long addr;  // Start of memory segment
	unsigned long long size;  // Size of memory segment
	unsigned long type;       // Type of memory segment
}e820_entry;

#endif
