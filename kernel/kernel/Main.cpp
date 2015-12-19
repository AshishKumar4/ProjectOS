#include <window.cpp>
extern "C"
void kernel_main(void)
{
    buffer=90 & 0xff;
    RectD(0,0,1021,767,90,90,90);
    int i=10,j=0;
    while(1)
    {
    if(i>1000)
        {i=0;j+=1;}
       // RectD(0,0,1021,767,90,90,90);
        Creater(i,j);
        mouse_handler();
        Mouse_Plot(MouseX(),MouseY());
       // irqHandler(mouse_handler);
        i+=1;
        RectD(500,400,13,100,1000,1000,1000);
        RectD(200,400,13,100,1000,1000,1000);
        DBuff();
    }
  //  printf("Hello world");
}

