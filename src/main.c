#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L
#include "locale.h"
#include "stdio.h"
#include "stdlib.h"

#include "time.h"
#include "unistd.h"
#include "ncurses.h"

#include "hachi.h"
#include "ncur_layer.h"
#include "x11_layer.h"

#include "hachi.c"
#include "ncur_layer.c"
#include "x11_layer.c"





int main(int argc, char **argv)
{

    float targetFPS = (1.0f/60.0f);

    if (argc < 2) return 1;

    setlocale(LC_ALL, "");
    FILE *fp = fopen(argv[1],"rb");
    fseek(fp, 0L, SEEK_END);
    int filesize = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    int offset = 0;
    int execIns = 0;
    for (int i=0; i < filesize; ++i)
    {
        unsigned char bf[2];
        fread(bf, sizeof(unsigned char), 2, fp);
        Hachi.mem[512+offset++] = bf[0];
        Hachi.mem[512+offset++] = bf[1];
        

    }
    
    

    setuphachi();


    while (!Hachi.close) {
        clock_t clk = clock();

        handleInput();
        handleResize();

        decodeAndExec(fetchIns());
        execIns++;


        clk = clock() - clk;

        if (targetFPS >  (float)clk /CLOCKS_PER_SEC && !(execIns < Hachi.insPerFrame))
        {
            execIns = 0;
            usleep( (targetFPS - (float) clk / CLOCKS_PER_SEC) * 1000000);
            
        }

        mvprintw(4, 0, "%f",(1-targetFPS - (float) clk / CLOCKS_PER_SEC)*60);
        mvprintw(5, 0, "InsPerFrame: %d",Hachi.insPerFrame);
    }

    atexit(cleanup);

    
    return 0;
}
