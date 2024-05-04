#include "ncurses.h"
#include "ncur_layer.h"

ncurInfo NcurInfo = {0};

void ncur_setupkeys()
{

    Hachi.kmap[0] = 'x';
    Hachi.kmap[1] = '1';
    Hachi.kmap[2] = '2';
    Hachi.kmap[3] = '3';
    Hachi.kmap[4] = 'q';
    Hachi.kmap[5] = 'w';
    Hachi.kmap[6] = 'e';
    Hachi.kmap[7] = 'a';
    Hachi.kmap[8] = 's';
    Hachi.kmap[9] = 'd';
    Hachi.kmap[10] = 'z';
    Hachi.kmap[11] = 'c';
    Hachi.kmap[12] = '4';
    Hachi.kmap[13] = 'r';
    Hachi.kmap[14] = 'f';
    Hachi.kmap[15] = 'v';
}

void ncur_setupscreen()
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr,1);

    NcurInfo.hascolor = has_colors();
    NcurInfo.blockUnicode = "@";

    if (NcurInfo.hascolor)
    {
        start_color();
        init_pair(COLOR_PAIR(0), COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_PAIR(1), COLOR_BLACK, COLOR_WHITE);
        NcurInfo.blockUnicode = " ";
        use_default_colors();
    }

    curs_set(0);
}


b32 checkScrSize()
{

    getmaxyx(stdscr, NcurInfo.h, NcurInfo.w);
    NcurInfo.centery = (NcurInfo.h/2) - dpy_h/2;
    NcurInfo.centerx = (NcurInfo.w/2) - dpy_w/2;

    if (NcurInfo.w < dpy_w || NcurInfo.h < dpy_h)
    {
        clear();
        mvprintw(0+NcurInfo.h/2,(0+NcurInfo.w/2)-7, "min size: %dx%d",dpy_w,dpy_h);
        return FALSE;
    }
    return TRUE;

}



void ncur_handleResize()
{
    int oldh = NcurInfo.h;
    int oldw = NcurInfo.w;

    getmaxyx(stdscr, NcurInfo.h, NcurInfo.w);

    if (oldh != NcurInfo.h || oldw != NcurInfo.w)
    {
        ncur_draw();
    }

}

void ncur_handleInput()
{

    char ch = getch();

    if (ch == 27) Hachi.close = TRUE;

    if (ch == Hachi.kmap[0]) Hachi.keys[0].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[1]) Hachi.keys[1].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[2]) Hachi.keys[2].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[3]) Hachi.keys[3].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[4]) Hachi.keys[4].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[5]) Hachi.keys[5].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[6]) Hachi.keys[6].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[7]) Hachi.keys[7].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[8]) Hachi.keys[8].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[9]) Hachi.keys[9].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[10]) Hachi.keys[10].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[11]) Hachi.keys[11].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[12]) Hachi.keys[12].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[13]) Hachi.keys[13].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[14]) Hachi.keys[14].timeout = Hachi.insPerFrame + 5;
    if (ch == Hachi.kmap[15]) Hachi.keys[15].timeout = Hachi.insPerFrame + 5;

    for (int i=0; i < 16; ++i)
    {
        if (Hachi.keys[i].timeout > 0 ) --Hachi.keys[i].timeout;

        Hachi.keys[i].state = Hachi.keys[i].timeout > 0;

        mvprintw(0 + i,0, "K%d st: %d", i, Hachi.keys[i].state);
        mvprintw((20 + i),0, "K%d t: %d", i, Hachi.keys[i].timeout);
    }

}

void ncur_end()
{
    echo();
    curs_set(1);
    endwin();
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
                    if (NcurInfo.hascolor)
                    {
                        color_set(COLOR_PAIR(1),WA_NORMAL);
                        mvaddstr(y+NcurInfo.centery,x+NcurInfo.centerx,NcurInfo.blockUnicode);
                        color_set(COLOR_PAIR(0),WA_NORMAL);
                    }
                    else
                    {
                        mvaddstr(y+NcurInfo.centery,x+NcurInfo.centerx,NcurInfo.blockUnicode);
                    }
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

