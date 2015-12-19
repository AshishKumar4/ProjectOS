;---------------------------------------------------------------------------------------------------------------------------
;McCabe OS Boot Loader (C) 2002 William McCabe
;---------------------------------------------------------------------------------------------------------------------------
;This code is the boot loader for McCabe OS
;---------------------------------------------------------------------------------------------------------------------------
; 3.5 Inch, 1.44 MB Floppy Disk
; 80 Cylinders, Two Heads = 1 for each side, 
; 18 Sectors Per Track, 2880 Sectors, 512 Bytes Per Sector

[BITS 16]               		; Start code in Real Mode (16 Bit)
[ORG 0x7C00]            		; Tell assembler to start at offset 0x7C00

        jmp short boot_code
        nop             		; Required nop

;---------------------------------------------------------------------------------------------------------------------------
; Bios parameter block
;---------------------------------------------------------------------------------------------------------------------------

        OsName                         	db "QUASI_OS"
        BytesPerSector                 	dw 0x200
        SectorsPerCluster              	db 0x01
        ReservedSectors                	dw 0x0001
        NumberOfFATs                   	db 0x02
        RootDirectoryEntries           	dw 0x00E0
        TotalSectors                   	dw 0x0B40
        MediaDescriptor                	db 0xF0
        SectorsPerFAT                 	dw 0x0009
        SectorsPerTrack                	dw 0x0012
        NumberOfHeads                  	dw 0x0002
        HiddenSectors                  	dd 0x00000000

        TotalSectorsHuge              	dd 0x00000000
        DriveNumber                    	db 0x00
        Reserved                       	db 0x00
        Signature                      	db 0x29
        VolumeName                     	db 0xFFFFFFFF
        FileSystemType                 	db "FAT12   "

;---------------------------------------------------------------------------------------------------------------------------
; Temporary Global Descriptor Table (T-GDT)
;---------------------------------------------------------------------------------------------------------------------------            
gdt:                                  	; Null Selector 0x0000
        dw 0
        dw 0
        db 0
        db 0
        db 0
        db 0
					
B_CodeSel equ $-gdt                   	; Bootloader Code Selector 0x0008 (Ring 0)

        dw 0xFFFF	              	; Limit 0xFFFF (1 MB, 4 GB)
	dw 00000000b	      	      	; Base 0
	db 00000000b                  	; Base 0
        db 10011010b	      	      	; Present, Ring 0, Code, Conforming, Readable 
	db 11001111b	      	      	; Page-Granular (4 GB Limit), 32-Bit
	db 00000000b            	; Base 0
             
B_DataSel equ $-gdt                  	; Bootloader Data Selector - 0x0010 (Ring 0)

        dw 0xFFFF		      	; Limit 0xFFFF (1 MB, 4 GB)
	dw 00000000b            	; Base 0
	db 00000000b           		; Base 0
	db 10010010b	      		; Present, Ring 0, Data, Expand-up, Writable
	db 11001111b	      		; Page-Granular (4 GB Limit), 32-Bit
	db 00000000b            	; Base 0  
gdt_end:

gdt_temp:
        dw gdt_end - gdt - 1    	; GDT Size (Limit)
        dd gdt                  	; GDT Location (Base)

;---------------------------------------------------------------------------------------------------------------------------
; Boot Code
;---------------------------------------------------------------------------------------------------------------------------
boot_code:

        xor ax, ax
        mov ds, ax

        mov [DriveNumber], dl
        
	; Setup a stack
	cli             	      	; Turn off interrupts while we setup a stack 
	mov ax, 0x9000   	      	; This seems to be the typical place for a stack
	mov ss, ax
	mov sp, 0xFFFE		      	; Let's use the whole segment - 1
	sti			      	; Turn interupts back on

;---------------------------------------------------------------------------------------------------------------------------
reset_Disk:			      	; Reset Floppy Disk

        xor ax, ax		      	; Zero AX register
	int 13h			      	; Actually Reset
        or  ah, ah                    	; Check for error code
       	jc  reset_Disk		      	; IF ERROR => reset again
	
;---------------------------------------------------------------------------------------------------------------------------
; Load sectors from the Floppy disk into memory
; Using CHS (Cylinder, Head, Sector) Addressing 
;---------------------------------------------------------------------------------------------------------------------------
readin_kernel:
 
	mov ax, 0xFFFF               	 ; ES:BX = 0xFFFF:0x0010 (kernel loaded at 1MB)
        mov es, ax
        mov bx, 0x0010               

	mov ah, 0x02		      	; Load disk data to ES:BX
	mov al, 0x17		      	; Load 17 sectors
	mov ch, 0x00		      	; Track  = 0x00
	mov cl, 0x02		      	; Sector = 0x02
	mov dh, 0x00		      	; Head   = 0x00
	mov dl, [DriveNumber]	      	; Drive number (0x00 = Floppy, 0x80 = Hard Drive)

       	int 13h			      	; Read Disk!
        or  ah, ah                    	; Check for error code
        jc  readin_kernel             	; If Error => read again

;---------------------------------------------------------------------------------------------------------------------------
; Set A20 Gate
;---------------------------------------------------------------------------------------------------------------------------

        cli			     	; Turn interupts off
        xor cx, cx

Clear_Buf:

        in al, 64h                    	; Get input from keyboard status port
        test al, 02h                  	; Test the buffer full flag
        loopnz Clear_Buf              	; Loop until buffer is empty
        mov al, 0D1h                  	; Keyboard: write to output port
        out 64h, al                   	; Output command to keyboard

Clear_Buf2:

        in al, 64h                    	; Wait till buffer is empty again
        test al, 02h
        loopnz Clear_Buf2
        mov al, 0dfh                  	; Keyboard: set A20
        out 60h, al                   	; Send it to the keyboard controller
        mov cx, 14h

Wait_Kbc:                             	; This is approx. a 25uS delay to wait

        out 0edh, ax                  	; For the kb controller to execute our 
        loop Wait_Kbc                 	; Command.

;---------------------------------------------------------------------------------------------------------------------------
; GET E820h Memory Map
;---------------------------------------------------------------------------------------------------------------------------

	push ds
	pop es
	mov di, 0x500
	xor ebx, ebx

get_e820:
	mov eax, 0x0000E820
	mov ecx, 0x20
	mov edx, 0x534D4150
	int 15h

	; CY = 1 on first call to int 15h, E820 is an error
	jc e820_end

	cmp eax, 0x534D4150
        jne e820_end

	add di, 20
	cmp ebx, 0
	jne get_e820
e820_end:  
     
;---------------------------------------------------------------------------------------------------------------------------
; Set Global Descriptor Table here, this must be done Prior to Protected Mode
;---------------------------------------------------------------------------------------------------------------------------

       	lgdt [gdt_temp]

;---------------------------------------------------------------------------------------------------------------------------	
; Lets Enter Protected Mode
;---------------------------------------------------------------------------------------------------------------------------

        mov eax, cr0		      	; Load the control register in
	or  eax, 0x01		      	; Set bit 1: pmode bit
	mov cr0, eax		      	; Copy it back to the control register
	
	jmp dword B_CodeSel:Protected_Mode

[BITS 32]
Protected_Mode:

        mov ax, B_DataSel
        mov ds, eax
        mov ss, eax
        nop                           	; No opperation
        mov es, eax
        mov fs, eax
        mov gs, eax

;---------------------------------------------------------------------------------------------------------------------------
; Turn Off Floppy Disk
;---------------------------------------------------------------------------------------------------------------------------

        mov edx, 0x3f2
        mov al, 0x0c
        out dx, al

        jmp dword B_CodeSel:0x100000 	; Jump to C kernel
       
;---------------------------------------------------------------------------------------------------------------------------

times 510-($-$$) db 0
dw 0xAA55
