#include "hachi.h"
#include "ncur_layer.h"
#include "x11_layer.h"

hachiChip  Hachi = {0};

unsigned char FONT_TABLE[16*5] = {
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void setuphachi()
{
    // Copy Font data from FONT_TABLE to Hachi MEM

    Hachi.backend = NCUR;
    int FontStAdr = 0x50;
    int FontEnAdr = 0x9F;
    int c = 0;

    for (int i=FontStAdr; i <= FontEnAdr; ++i)
    {
        Hachi.mem[i] = FONT_TABLE[c];
        ++c;
    }

    switch (Hachi.backend)
    {
        case NCUR: ncur_setupkeys();
                   ncur_setupscreen();
                   break;

        case X11_GL: x11_setupkeys();
           break;


        default: break;
    }
    Hachi.pc = 512;
}

unsigned short int fetchIns()
{
    short int ins = 0 | (Hachi.mem[Hachi.pc++] << 8) | (Hachi.mem[Hachi.pc++]);
    if (Hachi.pc > sizeof(Hachi.mem)) Hachi.pc = 512;
    return ins;
}

void decodeAndExec(unsigned short int ins)
{

    switch ((ins & (0xF000)))
    {

        case 0x0000:
            if (ins == 0x00E0) clearscreen();
            break;

        case 0x1000:
            Hachi.pc = (ins & (0x0FFF));
            break;

        case 0x2000:
            break;

        case 0x3000:
            break;

        case 0x4000:
            break;

        case 0x5000:
            break;

        case 0x6000:
            {
                char ind = 0 | ((ins & (0x0F00)) >> 8);
                Hachi.vreg[ind] = (ins & (0x00FF));
            }
            break;

        case 0x7000:
            {
                char ind = 0 | ((ins & (0x0F00)) >> 8);
                Hachi.vreg[ind] = Hachi.vreg[ind] == 255 ? 255 : Hachi.vreg[ind] + (ins & (0x00FF));
            }
            break;

        case 0x8000:
            break;

        case 0x9000:
            break;

        case 0xA000:
            {
                Hachi.ireg = (ins & (0x0FFF));
            }
            break;

        case 0xB000:
            break;

        case 0xC000:
            break;

        case 0xD000:
            {
                unsigned char inx = 0 | ((ins & (0x0F00)) >> 8);
                unsigned char iny = 0 | ((ins & (0x00F0)) >> 4);

                unsigned char x = Hachi.vreg[inx] & dpy_w;
                unsigned char y = Hachi.vreg[iny] & dpy_h;

                unsigned char sprsize = 0 | ((ins & (0x000F)));

                unsigned char xbyteindex = x/8;
                unsigned char xbitindex = x - (xbyteindex * 8);

                for (int i=0; i < sprsize; ++i)
                {
                    unsigned char sprbyte = Hachi.mem[Hachi.ireg +(i*8)];

                    unsigned char nbyteindex = xbyteindex;
                    unsigned char nbitindex = xbitindex;

                    for (int sprb = 0; sprb < 8; ++sprb)
                    {

                        if (sprbyte & (0x80 >> sprb)) {
                            
                            Hachi.dpy[(y*dpy_wb)+nbyteindex] ^= (1 << nbitindex); 

                            if (Hachi.dpy[(y*dpy_wb)+nbyteindex] & (0 << nbitindex))
                            {
                                Hachi.vreg[15] = 1;

                            }
                            
                        }
                        nbitindex++;
                        if (nbitindex > 7) nbitindex = 0, nbyteindex++;
                    }
                    y++;
                    if (y > dpy_h)
                        break;

                    
                }

                drawscreen();
            }
            break;

        case 0xE000:
            break;

        case 0xF000:
            break;
    }
}


void clearscreen()
{
    switch (Hachi.backend)
    {
        case NCUR:
            ncur_clear();
            break;

        case X11_GL:
              x11_clear();
              break;
        default:
              break;
    }
}

void drawscreen()
{
    switch (Hachi.backend)
    {
        case NCUR:
            ncur_draw();
            break;

        case X11_GL:
             x11_draw();
              break;
        default:
              break;
    }
}

void setupscreen()
{
    switch (Hachi.backend)
    {
        case NCUR:
            ncur_setupscreen();
            break;

        case X11_GL:
              x11_setupscreen();
              break;
        default:
              break;
    }
}
void cleanup()
{
    endwin();
}
