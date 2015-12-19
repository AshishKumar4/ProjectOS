OUTPUT_FORMAT("pei-i386") 
OUTPUT_ARCH(i386)
ENTRY(_start)

PHYSICAL_ADDRESS = 0x100000;
SECTIONS
{       
	. = PHYSICAL_ADDRESS;

       	.text : 
        {              
		kernel_code = .;  
		*(.text)
		. = ALIGN(4096);
	}

	.data : 
	{     
		kernel_data = .;           
     		*(.data)
		. = ALIGN(4096);
	}

	.bss : 
	{          
		kernel_bss = .;      
		*(.bss)
		*(COMMON)
		. = ALIGN(4096);		
	}
        kernel_end = .;	
}