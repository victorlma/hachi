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
    Hachi.insPerFrame = 5;
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
//    if (Hachi.pc > sizeof(Hachi.mem)) Hachi.pc = 512;
    return ins;
}


void decodeAndExec(unsigned short int ins)
{

    switch ((ins & (0xF000)))
    {

        case 0x0000:
            if (ins == 0x00E0) clearscreen();
            if (ins == 0x00EE) 
            {
                Hachi.pc = Hachi.stack[Hachi.stackCursor-1];
                Hachi.stackCursor--;
            }
            break;

        case 0x1000:
            Hachi.pc = (ins & (0x0FFF));
            break;

        case 0x2000:
            Hachi.stack[Hachi.stackCursor++] = Hachi.pc;
            Hachi.pc = (ins & (0x0FFF));
            break;

        case 0x3000:
            {
                unsigned char x = ((ins & (0x0F00)) >> 8);
                unsigned char nn = (ins & (0x00FF));

                if (Hachi.vreg[x] == nn) Hachi.pc += 2;
            }

            
            break;

        case 0x4000:
            {
                unsigned char x = ((ins & (0x0F00)) >> 8);
                unsigned char nn = (ins & (0x00FF));

                if (Hachi.vreg[x] != nn) Hachi.pc += 2;
            }
            break;

        case 0x5000:
            {
                unsigned char x = ((ins & (0x0F00)) >> 8);
                unsigned char y = ((ins & (0x00F0)) >> 4);

                if (Hachi.vreg[x] == Hachi.vreg[y]) Hachi.pc += 2;
            }
            break;

        case 0x6000:
            {
                unsigned char ind = 0 | ((ins & (0x0F00)) >> 8);
                Hachi.vreg[ind] = (ins & (0x00FF));
            }
            break;

        case 0x7000:
            {
                unsigned char ind = 0 | ((ins & (0x0F00)) >> 8);
                Hachi.vreg[ind] += (ins & (0x00FF));
            }
            break;

        case 0x8000:
            switch (ins & 0x000F)
            {
                case 0x0000:
                    {
                        unsigned char x = ((ins & (0x0F00)) >> 8);
                        unsigned char y = ((ins & (0x00F0)) >> 4);
                        Hachi.vreg[x] = Hachi.vreg[y];
                    }
                    break;
                case 0x0001:
                    {
                        unsigned char x = ((ins & (0x0F00)) >> 8);
                        unsigned char y = ((ins & (0x00F0)) >> 4);
                        Hachi.vreg[x] |= Hachi.vreg[y];
                    }
                    break;
                case 0x0002:
                    {
                        unsigned char x = ((ins & (0x0F00)) >> 8);
                        unsigned char y = ((ins & (0x00F0)) >> 4);
                        Hachi.vreg[x] &= Hachi.vreg[y];
                    }
                    break;
                case 0x0003:
                    {
                        unsigned char x = ((ins & (0x0F00)) >> 8);
                        unsigned char y = ((ins & (0x00F0)) >> 4);
                        Hachi.vreg[x] ^= Hachi.vreg[y];
                    }
                    break;
                case 0x0004:
                    {
                        unsigned char x = ((ins & (0x0F00)) >> 8);
                        unsigned char y = ((ins & (0x00F0)) >> 4);
                        Hachi.vreg[x] += Hachi.vreg[y];
                        
                        Hachi.vreg[15] = Hachi.vreg[x] < Hachi.vreg[y] ? 1 : 0;
                    }
                    break;
                case 0x0005:
                    {
                        unsigned char x = ((ins & (0x0F00)) >> 8);
                        unsigned char y = ((ins & (0x00F0)) >> 4);


                        unsigned char tmp = Hachi.vreg[x];
                        Hachi.vreg[x] -= Hachi.vreg[y];
                        Hachi.vreg[15] =  tmp >= Hachi.vreg[y] ? 1 : 0;

                    }
                    break;
                case 0x0006:

                    switch (Hachi.quirk) {
                        case CHIP8:
                        {
                            unsigned char x = ((ins & (0x0F00)) >> 8);
                            unsigned char y = ((ins & (0x00F0)) >> 4);

                            Hachi.vreg[x] = Hachi.vreg[y];
                            unsigned char tmpcarry  = Hachi.vreg[x] & 1;
                            Hachi.vreg[x] >>= 1;
                            Hachi.vreg[15] = tmpcarry;

                        }
                            break;
                        case CHIP48:
                        case SUPERCHIP:
                        {
                            unsigned char x = ((ins & (0x0F00)) >> 8);

                            unsigned char tmpcarry  = Hachi.vreg[x] & 1;
                            Hachi.vreg[x] >>= 1;
                            Hachi.vreg[15] = tmpcarry;

                        }
                            break;
                        default:
                            break;
                    }
                    break;

                case 0x0007:
                    {
                        unsigned char x = ((ins & (0x0F00)) >> 8);
                        unsigned char y = ((ins & (0x00F0)) >> 4);

                        unsigned char tmp = Hachi.vreg[y];
                        Hachi.vreg[x]= Hachi.vreg[y] - Hachi.vreg[x];
                        Hachi.vreg[15] =  tmp > Hachi.vreg[x] ? 1 : 0;


                    }
                    break;
                case 0x000E:
                    switch (Hachi.quirk) {
                        case CHIP8:
                        {
                            unsigned char x = ((ins & (0x0F00)) >> 8);
                            unsigned char y = ((ins & (0x00F0)) >> 4);

                            Hachi.vreg[x] = Hachi.vreg[y];
                            unsigned char tmpcarry  = Hachi.vreg[x] >> 7;
                            Hachi.vreg[x] <<= 1;
                            Hachi.vreg[15] = tmpcarry;

                        }
                            break;
                        case CHIP48:
                        case SUPERCHIP:
                        {
                            unsigned char x = ((ins & (0x0F00)) >> 8);

                            unsigned char tmpcarry  = Hachi.vreg[x] >> 7;
                            Hachi.vreg[x] <<= 1;
                            Hachi.vreg[15] = tmpcarry;

                        }
                            break;
                        default:
                            break;
                    }
                    break;
            }
            break;

        case 0x9000:
            {
                unsigned char x = ((ins & (0x0F00)) >> 8);
                unsigned char y = ((ins & (0x00F0)) >> 4);

                if (Hachi.vreg[x] != Hachi.vreg[y]) Hachi.pc += 2;
            }
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

                unsigned char x = (Hachi.vreg[inx] & (dpy_w-1));
                unsigned char y = (Hachi.vreg[iny] & (dpy_h-1));

                unsigned char sprsize = 0 | ((ins & (0x000F)));



                for (int i=0; i < sprsize; ++i) {
                    unsigned char sprb = Hachi.mem[Hachi.ireg+i];

                    unsigned char xbyteindex = x/8;
                    unsigned char xbitindex = x - (xbyteindex * 8);

                    unsigned char bitspraux = 0;

                    for (int byteinspr = 0; byteinspr < 8; ++byteinspr)
                    {
                        if ((xbitindex + bitspraux) > 7)
                        {
                            xbitindex = 0;
                            xbyteindex += 1;
                            bitspraux = 0;
                        }
                        unsigned char *dpybyte = &Hachi.dpy[((y+i)*dpy_wb) + xbyteindex];

                        if (sprb & ( 0x80 >> byteinspr))
                        {
                            *dpybyte ^= (0x80 >> (xbitindex + bitspraux));

                            if (!(*dpybyte & (0x80 >> (xbitindex + bitspraux))))
                            {
                                Hachi.vreg[15] = 1;
                            }
                        }
                        bitspraux++;
                    }

                }
                drawscreen();
            }
            break;

        case 0xE000:
            break;

        case 0xF000:
            switch (ins & 0x00FF)
            {
                case 0x001E:
                    {
                        unsigned char indx = ((ins & 0x0F00) >> 8);
                        Hachi.ireg += Hachi.vreg[indx];
                    }
                    break;
                case 0x0033:
                    {
                        unsigned char indx = ((ins & 0x0F00) >> 8);

                        Hachi.mem[Hachi.ireg] = Hachi.vreg[indx] / 100;
                        Hachi.mem[Hachi.ireg+1] = (Hachi.vreg[indx] % 100)  / 10;
                        Hachi.mem[Hachi.ireg+2] = (Hachi.vreg[indx] % 100) % 10;
                    }
                    break;

                case 0x0055:
                    {
                        unsigned char indx = ((ins & 0x0F00) >> 8);

                        for (int i=0; i <= indx; ++i)
                        {
                            Hachi.mem[Hachi.ireg+i] = Hachi.vreg[i];
                        }

                    }
                    break;
                case 0x0065:
                    {
                        unsigned char indx = ((ins & 0x0F00) >> 8);

                        for (int i=0; i <= indx; ++i)
                        {
                            Hachi.vreg[i] = Hachi.mem[Hachi.ireg+i];
                        }

                    }
                    break;
            }
            break;
    }
}


void clearscreen()
{
    for (int i=0; i < dpy_wb * dpy_hb; ++i)
    {
        Hachi.dpy[i] = 0;
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

void handleInput()
{

    switch (Hachi.backend)
    {
        case NCUR: ncur_handleInput();
            break;

        case X11_GL:
           break;

        default: break;
    }
}

void handleResize()
{
    switch (Hachi.backend)
    {
        case NCUR: ncur_handleResize();
            break;

        case X11_GL:
           break;

        default: break;
    }
}
void cleanup()
{
    switch (Hachi.backend)
    {
        case NCUR: ncur_end();
            break;

        case X11_GL:
           break;

        default: break;
    }
}
