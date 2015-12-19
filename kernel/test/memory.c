#include "mm.h"
#include "stdio.h"

unsigned long reserved_memory = 0;
unsigned long useable_memory  = 0;
extern unsigned int read_cr0;

void init_mm(void)
{
		printk("Initializing Memory Management\n");

		// Figure out amount of memory
		get_e820Map();

		// Initialize Paging
		init_paging();
}

// Gets the memory map and determines the correct amount of memory
void get_e820Map(void)
{
		e820_entry *mem = (e820_entry*) 0x500;
		unsigned long a, map_length;

		asm volatile("movl %%edi,%0" : "=r" (map_length));
		for(a = 0; a < (map_length - 0x500) / 20; a++)
		{
			if(mem[a].type==1)
			{
				useable_memory += mem[a].size;
			}
			else
			{
				reserved_memory += mem[a].size;
			}
		}

		printk("Reserved: %iKB, Useable: %iKB, Total Memory: %iMB\n", reserved_memory / 1024, useable_memory / 1024, (reserved_memory + useable_memory) / 1024 / 1024);
		printk("Max Pages for Physical RAM: [%i]\n", (reserved_memory + useable_memory) / 4096 + 1);
}

// Setup the the boot page directory and tables, and enable paging
void init_paging(void)
{
		printk("Initializing Paging Mode\n");

        unsigned long *page_directory = (unsigned long *) 0x124000; // Page directory
		unsigned long *page_table = (unsigned long *) 0x125000;     // Page table

        unsigned long address = 0;					// Address to start mapping from
		unsigned long i;

		//********************************************************************************************************
		// Setup page directory
		//********************************************************************************************************

		// Fill the first entry of the page directory
		page_directory[0] = (long) page_table;
		page_directory[0] = page_directory[0] | 3;	// Attribute set: supervisor level, read/write, present (011)

		// Page_directory = 1024 page entrys (page tables), set all but 1 to be not_present
		// Trying to access a page table that is not_present results in a page fault
		for(i = 1; i < 1024; i++);
		{
			page_directory[i] = 0 | 2;				// Attribute set: supervisor level, not present (010)
		}

		//********************************************************************************************************
		// Map the page table
		//********************************************************************************************************

		// Map the first 2MB of ram (512 pages)
		for(i = 0; i < 512; i++)
		{
			page_table[i] = address | 3;             // Attribute set: supervisor level, read/write, present (011)
            address += 4096;
		}

		// Set the rest to not present (512 pages)
		for(i = 512; i < 1024; i++)
		{
            page_table[i] = address | 2;             // Attribute set: supervisor level, not present (010)
		}

		//********************************************************************************************************
		// Enable paging
		//********************************************************************************************************
		write_cr3(page_directory);			         // Put page directory address into CR3
		write_cr0(read_cr0 | 0x80000000);	         // Set paging bit in CR0 to 1 (Enabled)

		printk("Kernel is now in Paging Mode\n");
}
