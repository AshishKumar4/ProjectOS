##############################################################################
# Basic Makefile for IBOX OS, Ver.0.02, Date: 06/05/04 10:00PM
##############################################################################

CC       = gcc
LD       = ld
NASM     = nasm
OBJ      = objcopy

CFLAGS   = -ffreestanding -fno-builtin -mno-stack-arg-probe -I./include -Wall
LDFLAGS  = -T linker.ld 
OBJFLAGS = -O binary kernel.exe kernel.bin

OBJS = stub.o kernel.o 8259.o console.o exceptions.o funct.o handlers.o init_interrupts.o io.o memory.o stdio.o string.o timer.o vsprintf.o 

default: noclean install clean
noclean: bootpm.bin stub.o kernel.o 8259.o console.o exceptions.o funct.o handlers.o init_interrupts.o io.o memory.o stdio.o string.o timer.o vsprintf.o kernel.exe

##############################################################################
# Assemble all the Asm files
##############################################################################
bootpm.bin: bootpm.asm
	$(NASM) -f bin bootpm.asm -o bootpm.bin
funct.o: funct.asm
	$(NASM) -f gnuwin32 funct.asm -o funct.o
handlers.o:handlers.asm
	$(NASM) -f gnuwin32 handlers.asm -o handlers.o
stub.o: stub.asm
	$(NASM) -f gnuwin32 stub.asm -o stub.o

##############################################################################
# Compile all C files
##############################################################################
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c
8259.o: 8259.c
	$(CC) $(CFLAGS) -c 8259.c
console.o: console.c
	$(CC) $(CFLAGS) -c console.c
exceptions.o: exceptions.c
	$(CC) $(CFLAGS) -c exceptions.c
init_interrupts.o: init_interrupts.c
	$(CC) $(CFLAGS) -c init_interrupts.c
io.o: io.c
	$(CC) $(CFLAGS) -c io.c
memory.o: memory.c
	$(CC) $(CFLAGS) -c memory.c
stdio.o: stdio.c
	$(CC) $(CFLAGS) -c stdio.c
string.o: string.c
	$(CC) $(CFLAGS) -c string.c
timer.o: timer.c
	$(CC) $(CFLAGS) -c timer.c
vsprintf.o: vsprintf.c
	$(CC) $(CFLAGS) -c vsprintf.c

#############################################################################
# Link them all together
#############################################################################
kernel.exe: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.exe $(OBJS)
	$(OBJ) $(OBJFLAGS) 

install:
	dd if=bootpm.bin of=/dev/fd0
	dd if=kernel.bin of=/dev/fd0 seek=1
	
	cat bootpm.bin kernel.bin > ibox.img
	#cp ibox.img /bochs-2.0.2/
	
clean:
	rm *.bin
	rm *.exe
	rm *.o
	#rm *.img
	
	
	
