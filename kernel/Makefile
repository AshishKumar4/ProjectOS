HOST?=$(shell ../default-host.sh)
HOSTARCH:=$(shell ../target-triplet-to-arch.sh $(HOST))

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_myos_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR:=arch/$(HOSTARCH)
C:=c/

include $(ARCHDIR)/make.config
V:=$(C)/v86.o
IDT:=$(C)/idt.o
INIT:=$(C)/init.o
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
CPPALL:=$(C)/cpp.o
x86_SUPPORT:=$(C)/*.o
UTILS:=$(C)/utils.o
WINDOW:=include/window.o
UTILS1:=$(ARCHDIR)/utils.o
VESA:=include/vesa.o
OBJS:=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o
MAIN:=kernel/Main.cpp
MAINOBJ:=kernel/main.o
CRTI_OBJ:=$(ARCHDIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(ARCHDIR)/crtn.o

ALL_OUR_OBJS:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ) \

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \

all: build-cpp-nasm myos.kernel

.PHONY: all clean install install-headers install-kernel

myos.kernel: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(MAINOBJ) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS) $(x86_SUPPORT)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f myos.kernel $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o

install: build-cpp-nasm install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)

build-cpp-nasm:
	nasm -f elf nasm/v86.asm -o $(V)
	nasm -f elf nasm/idt.s -o $(IDT)
	nasm -f elf nasm/init.s -o $(INIT)
	i686-elf-g++ -c $(MAIN) -o $(MAINOBJ) -Iinclude
