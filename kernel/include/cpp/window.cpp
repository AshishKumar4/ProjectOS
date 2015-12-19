//#include <mouse.c>
extern void RectD(int x, int y, int width, int height, int C1,int C2, int C3);
 class Window
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

};

