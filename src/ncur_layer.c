#include "ncurses.h"
#include "ncur_layer.h"

ncurInfo NcurInfo = {0};

void ncur_setupkeys()
{
    Hachi.keys[0].key = '1' ;
    Hachi.keys[1].key = '2' ;
    Hachi.keys[2].key = '3' ;
    Hachi.keys[3].key = '4' ;
    Hachi.keys[4].key = 'q' ;
    Hachi.keys[5].key = 'w' ;
    Hachi.keys[6].key = 'e' ;
    Hachi.keys[7].key = 'r' ;
    Hachi.keys[8].key = 'a' ;
    Hachi.keys[9].key = 's' ;
    Hachi.keys[10].key = 'd' ;
    Hachi.keys[11].key = 'f' ;
    Hachi.keys[12].key = 'z' ;
    Hachi.keys[13].key = 'x' ;
    Hachi.keys[14].key = 'c' ;
    Hachi.keys[15].key = 'v' ;
}

void ncur_setupscreen()
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
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

b32 charInArr(char ch, char arr[], int size)
{
    b32 is = FALSE;

    for (int i = 0; i < size; ++i)
    {
        if (ch == arr[i]) is = TRUE;
    }

    return is;
}

b32 charIsKey(char ch)
{
    b32 is = FALSE;

    for (int i = 0; i < 16; ++i)
    {
        if (ch == Hachi.keys[i].key) is = TRUE;
    }

    return is;
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
    for (int i = 0; i < 16; ++i)
    {
        NcurInfo.charr[i] = 0;

    }

    for (int i = 0; i < 16; ++i)
    {
        NcurInfo.ch = getch();

        if (NcurInfo.ch == 27)
        {
            Hachi.close = TRUE;
            return;
        }
        
        if (charIsKey(NcurInfo.ch))
        {
        
            if (charInArr(NcurInfo.ch, NcurInfo.charr, i+1))
            {
                NcurInfo.ch = ERR;
            }

            if (NcurInfo.ch != ERR) NcurInfo.charr[i] = NcurInfo.ch;
        }
        
    }

    for (int i = 0; i < 16; ++i)
    {
        Hachi.keys[i].state = charInArr(Hachi.keys[i].key, NcurInfo.charr, 16) ? TRUE : FALSE;
    }
}

void ncur_end()
{
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

