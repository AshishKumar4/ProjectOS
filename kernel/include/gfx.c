#include <vesa.c>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <font.c>


extern int widthVESA, heightVESA, depthVESA;
unsigned int background=90;
unsigned char *tempbuff;
unsigned char buffer;
extern unsigned char *vga_mem; //pointer where we assign our vga address
void Init()
{
    //RectL(0,0,1022,767,90,90,90);
}

//void Clear()
//{
 //   for(int i=0;i<(1020*(depthVESA/8)+760*(widthVESA*(depthVESA/8)));i++)
  //      vga_mem[i]=NULL;
//}
void Pixel_VESA_BUFF(int x, int y, int C1,int C2, int C3)
{
  /*calculates the offset for a specific point on screen*/
    int offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));

    buff[offset+1]=C1& 0xff;
    buff[offset+2]=C2& 0xff;
    buff[offset+3]=C3& 0xff;

}
int offset;
void Pixel_VESA(int x, int y, int C1,int C2, int C3)
{
  /*calculates the offset for a specific point on screen*/
    offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));
    vga_mem[offset + 0] = C1 & 0xff;           //BLUE
    vga_mem[offset + 1] = C2 & 0xff;    //GREEN
    vga_mem[offset + 2] = C3 & 0xff;   //RED*/

   // buffer1[offset] = C1;           //BLUE
  //  buffer2[offset] = C2;    //GREEN
   // buffer3[offset] = C3;   //RED
    buff[offset+1]=C1& 0xff;
    buff[offset+2]=C2& 0xff;
    buff[offset+3]=C3& 0xff;
}

void RectD(int x, int y, int width, int height, int C1,int C2, int C3)
{
  for(int i = y; i < height + y; i++)
    for(int j = x; j < width + x; j++)
      Pixel_VESA_BUFF(j, i, C1,C2,C3);
}
int width=1022,height=766;
void DBuff()
{
    memcpy(vga_mem,buff,1024*768*2);
   /* for(int i = 0; i < height; i++)
    for(int j = 0; j < width; j++)
    {
        offset = j * (depthVESA / 8) + i * (widthVESA * (depthVESA / 8));
        if(buff[offset]!=vga_mem[offset])
        {
            vga_mem[offset + 0] = buff[offset];
        }
        if(buff[offset+1]!=vga_mem[offset+1])
        {
            vga_mem[offset + 1] = buff[offset+1];
        }
        if(buff[offset+2]!=vga_mem[offset+2])
        {
            vga_mem[offset + 2] = buff[offset+2];
        }
    }*/
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
    {
        offset = j * (depthVESA / 8) + i * (widthVESA * (depthVESA / 8));
        if(buff[offset]!=buffer)
           buff[offset]=buffer;
        if(buff[offset+1]!=buffer)
           buff[offset+1]=buffer;
        if(buff[offset+2]!=buffer)
           buff[offset+2]=buffer;
    }
}
void RectL(int x, int y, int width, int height, int C1,int C2, int C3)
{
  for(int i = y; i < height + y; i++)
    for(int j = x; j < width + x; j++)
      Pixel_VESA(j, i, C1,C2,C3);
}
void Creater(int i,int j)
{
    Pixel_VESA(i,i+j,1000,1000,1000);
}
void Mouse_Plot(int x,int y)
{
    for(int i=0;i <20;i++)
    {
        if(i<13)
            for (int j=0;j <i;j++)
            {
                Pixel_VESA_BUFF(x+j,y+i,90,10000,10000);
            }
        else
            for(float j=0;j<19-i;j=j+0.6)
            {
                Pixel_VESA_BUFF(x+j,y+i,90,10000,10000);
            }
    }
}
//the vga identifiers
u32int VGA_width=768;
u32int VGA_height=1024;
u32int VGA_bpp=64;

