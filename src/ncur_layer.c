#include "ncurses.h"
#include "ncur_layer.h"

char *blockUnicode = "\u2588";

void ncur_setupkeys()
{ 
    Hachi.k.b0.key = '1' ;
    Hachi.k.b1.key = '2' ;
    Hachi.k.b2.key = '3' ;
    Hachi.k.b3.key = '4' ;
    Hachi.k.b4.key = 'q' ;
    Hachi.k.b5.key = 'w' ;
    Hachi.k.b6.key = 'e' ;
    Hachi.k.b7.key = 'r' ;
    Hachi.k.b8.key = 'a' ;
    Hachi.k.b9.key = 's' ;
    Hachi.k.ba.key = 'd' ;
    Hachi.k.bb.key = 'f' ;
    Hachi.k.bc.key = 'z' ;
    Hachi.k.bd.key = 'x' ;
    Hachi.k.be.key = 'c' ;
    Hachi.k.bf.key = 'v' ;
}

void ncur_setupscreen()
{
    initscr();

}

b32 checkScrSize()
{

    int w;
    int h;
    getmaxyx(stdscr, h, w);
    int centery = h/2;
    int centerx = w/2;

    if (w < dpy_w || h < dpy_h)
    {
        clear();
        mvprintw(0+centery,0+centerx-7, "min size: %dx%d",dpy_w,dpy_h);
        return FALSE;
    }
    return TRUE;

}
void ncur_draw()
{
    clear();
    if (checkScrSize())
    {
        clear();
        int y = 0;
        int x = 0;

        int c = 0;
        for (int dpybyte=0; dpybyte < dpy_wb* dpy_h; ++dpybyte)
        {
            unsigned char curbyte = Hachi.dpy[dpybyte];
            for (int i=0; i < 8; ++i)
            {
                if (curbyte & (0x80 >> i))
                {
                    mvaddstr(y,x,blockUnicode);
                }
                ++x;
            }

            ++c;
            if (c > 7) 
            {
                c=0;
                ++y;
                x=0;
            }
        }
    }
    refresh();
}

