;---------------------------------------------------------------------------------------------------------------------------
; Protected Mode test kernel
;---------------------------------------------------------------------------------------------------------------------------
SECTION .text                           ; .TEXT section
[BITS 32]
					
GLOBAL _read_cr0, _write_cr0, _read_cr3, _write_cr3

_read_cr0:
	mov eax, cr0
	retn

_write_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]
	mov cr0, eax
	pop ebp
	retn

_read_cr3:
	mov eax, cr3
	retn

_write_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]
	mov cr3, eax
	pop ebp
	retn
