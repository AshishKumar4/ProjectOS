//#include <mouse.c>

extern "C"
{
    unsigned char buffer;
    void RectD(int x,int y, int width, int height, int C1, int C2, int C3 );
    void Creater(int i, int j);
    void mouse_handler();
    void Mouse_Plot(int x, int y);
    void DBuff();
    int MouseX();
    int MouseY();
    int printf(const char*, ...);
    typedef void (*void_callback_arg_t)(unsigned int, ...);
    struct regs
    {
        unsigned int gs, fs, es, ds;      /* pushed the segs last */
        unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
        unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
        unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
    };
    void irq_register_install(unsigned char a,void (*f)(regs* r));
}
typedef class Window
{
    public:
    int Width;
    int Height;
    int BackColor;
    struct buttons
    {
        int x;
        int y;
        int function;
    }close;
    void Draw(int x,int y)
    {
        RectD(x,y,Width,Height,BackColor,BackColor,BackColor);
        RectD(x,y,Width,30,50,50,50);
    }

}New;
