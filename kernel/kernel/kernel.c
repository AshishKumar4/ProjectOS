#include <kernel/tty.h>
#include <mouse.c>
#include <timer.c>
char bc=0;
void irq_register_install(unsigned char a,void (*f)(struct regs* r))
{
    interruptHandlerRegister(a,&f);
}
void kernel_early(void)
{
    terminal_initialize();
    printf("Terminal Initialized! \n");
    kmalloc(1);
    printf("Heap Initialized! \n");
    gdtInit();
    printf("GDT Initialized \n");
    init_interrupts();

    printf("IDT Initialized \n");
    picInit(0x20, 0xA0);
    printf("PIC Initialized \n");
    //mouseinit();
    printf("Mouse Initialized \n");
    printf("Initializing VESA GUI Mode 1024 x 768 32000^3 colors");
    //setVesa(0x117);
    mouseinit();
}
void kernel_start(void)
{
}
int main() //ignore, its nothing
{
    return 0;
}
int MouseX()
{
    return mousex;
}
int MouseY()
{
    return mousey;
}

