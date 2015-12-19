[BITS 32]

GLOBAL divide_error 
GLOBAL debug_exception 
GLOBAL non_maskable_interrupt 
GLOBAL breakpoint
GLOBAL overflow
GLOBAL bounds_check
GLOBAL invalid_opcode 
GLOBAL coprocessor_not_available
GLOBAL double_fault
GLOBAL coprocessor_segment_overrun
GLOBAL invalid_tss
GLOBAL segment_not_present
GLOBAL stack_exception
GLOBAL general_protection_fault
GLOBAL page_fault
GLOBAL reserved
GLOBAL coprocessor_error
GLOBAL alignment_check
GLOBAL machine_check

EXTERN _fault_handler
%macro STUB 1

	push byte 0			; "Fake" error code
	push byte %1		        ; Exception number
		
	pusha			
	push ds
	push es
	push gs
	push fs
	
	mov ax, 0x10
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax

        push eax

        mov eax, _fault_handler
	call eax

        pop eax

	pop gs
	pop fs
	pop es
	pop ds
	popa

	add esp, 8
	iret
%endmacro

;---------------------------------------------------------------------------------------------------------------------------
; Exception Handlers
;---------------------------------------------------------------------------------------------------------------------------
; ISR00 (Divide Error)
divide_error:

      	STUB 0
        
; ISR01 (Debug Exception)
debug_exception:
	
	STUB 1

; ISR02 (NMI Interrupt)
non_maskable_interrupt:

	STUB 2

; ISR03 (Breakpoint)
breakpoint:

	STUB 3

; ISR04 (Overflow)
overflow:

	STUB 4

; ISR05 (Bounds Check)
bounds_check:

	STUB 5

; ISR06 (Invalid Opcode)
invalid_opcode:
	
	STUB 6

; ISR07 (Coprocessor Not Available)
coprocessor_not_available:

	STUB 7

; ISR08 (Double Fault)
double_fault:

	STUB 8

; ISR09 (Coprocessor Segment Overrun)
coprocessor_segment_overrun:

	STUB 9

; ISR10 (Invalid TSS)
invalid_tss:

	STUB 10

; ISR11 (Segment Not Present)
segment_not_present:

	STUB 11

; ISR12 (Stack Exception)
stack_exception:

	STUB 12

; ISR13 (General Protection Fault)
general_protection_fault:

	STUB 13

; ISR14 (Page Fault)
page_fault:

	pusha
	push ds
	push es
	push fs
	push gs

	mov eax, cr2			; CR2 contains the address that the program tried to access
	push eax			; Have our C function get it
					
	call page_fault
	pop eax			
	
	pop gs
	pop fs
	pop es
	pop ds
	popa
	iret

; ISR15 (Reserved)
reserved:

	STUB 15

; ISR16 (Coprocessor Error)
coprocessor_error:

	STUB 16

; ISR17 (Alignment Check)
alignment_check:

	STUB 17

; ISR18 (Machine Check)
machine_check:

	STUB 18

;---------------------------------------------------------------------------------------------------------------------------
; INTERRUPT HANDLERS
;---------------------------------------------------------------------------------------------------------------------------
; IRQ00 (Timer Interrupt)
[GLOBAL irq00]
[EXTERN _timer_handler]
irq00:
	pusha
	push ds
	push es
	push fs
	push gs

	call _timer_handler

	mov al, 0x20
	out 0x20, al

	pop gs
	pop fs
	pop es
	pop ds
	popa
	iret