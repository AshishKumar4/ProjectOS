#include <task.cpp>
void Desktop()
{
    buffer=90& 0xff;
    RectD(0,0,1023,767,90,90,90);
    while(1)
    {
        mouse_handler();
        Mouse_Plot(MouseX(),MouseY());
        DBuff();
    }
}
