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
