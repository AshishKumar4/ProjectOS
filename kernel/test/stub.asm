;---------------------------------------------------------------------------------------------------------------------------
; Protected Mode test kernel
;---------------------------------------------------------------------------------------------------------------------------
SECTION .text                           ; .TEXT section
[BITS 32]
										
GLOBAL _start
EXTERN _kmain

_start:
       	mov ax,FLAT_DATA_SEL	        ; Update segment registers
	mov ds, ax

	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

        lgdt [gdt_table]
        jmp next_Set 

next_Set: 
	
	; Zero BSS
	extern kernel_bss, kernel_end	

	mov edi,kernel_bss
	mov ecx,kernel_end
	sub ecx,edi
	xor eax,eax
	rep stosb 

       	mov esp,stack_top	        ; Set up our stack

	call _kmain
        jmp $

SECTION .data				; .DATA section
;---------------------------------------------------------------------------------------------------------------------------
; Permanent Global Descriptor Table (P-GDT)
;---------------------------------------------------------------------------------------------------------------------------
gdt:          				; Null Selector 0x0000
        dw 0
        dw 0
        db 0
        db 0
        db 0
        db 0
					
FLAT_CODE_SEL equ $-gdt                 ; Bootloader Code Selector 0x0008 (Ring 0)

        dw 0xFFFF	                ; Limit 0xFFFF (1 MB, 4 GB)
	dw 0	                        ; Base 0
	db 0                            ; Base 0
	db 0x9A	                        ; Present, Ring 0, Code, Conforming, Readable 
	db 0xCF	                        ; Page-Granular (4 GB Limit), 32-Bit
        db 0                            ; Base 0
             
FLAT_DATA_SEL equ $-gdt                 ; Bootloader Data Selector - 0x0010 (Ring 0)

        dw 0xFFFF                       ; Limit 0xFFFF (1 MB, 4 GB)
        dw 0                            ; Base 0
	db 0                            ; Base 0
	db 0x92                         ; Present, Ring 0, Data, Expand-up, Writable
	db 0xCF    	                ; Page-Granular (4 GB Limit), 32-Bit
	db 0                            ; Base 0  
gdt_end:

gdt_table:
        dw gdt_end - gdt - 1            ; GDT Size (Limit)
        dd gdt                          ; GDT Location (Base)

;---------------------------------------------------------------------------------------------------------------------------
SECTION .bss				; .BSS section 

stack:  resb 4096						
stack_top:


