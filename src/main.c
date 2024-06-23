#define _POSIX_C_SOURCE 199309L 
#include "locale.h"
#include "stdio.h"
#include "stdlib.h"

#include "time.h"
#include "ncurses.h"

#include "hachi.h"
#include "ncur_layer.h"
#include "x11_layer.h"

#include "hachi.c"
#include "ncur_layer.c"
#include "x11_layer.c"





int main(int argc, char **argv)
{

    if (argc < 2) return 1;

    setlocale(LC_ALL, "");
    FILE *fp = fopen(argv[1],"rb");
    fseek(fp, 0L, SEEK_END);
    int filesize = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    int execIns = 0;

    for (int i=0; i < filesize; ++i)
    {
        unsigned char bf[2];
        fread(bf, sizeof(unsigned char), 2, fp);
        Hachi.mem[512+i++] = bf[0];
        Hachi.mem[512+i] = bf[1];

    }
    
    
    setuphachi();

    float target = 1.0f/Hachi.fps;

    struct timespec tspec = {0};



    clock_t clk = clock();
    float ms = 0;
    while (!Hachi.close) {


        handleInput();
        clock_t clkacc = 0;
        while (execIns < Hachi.insPerFrame)
        {
            handleResize();
            decodeAndExec(fetchIns());
            execIns++;



            clk = clock() - clk;

            clkacc += clk;
            ms = (float) clkacc/CLOCKS_PER_SEC;
            if (ms >= target) break;

            clk = clock();
        }

        if (ms < target)
        {
            tspec.tv_sec = 0;
            tspec.tv_nsec = (target - ms) * 1000000000L;

            nanosleep(&tspec, &tspec);
        }

        if (Hachi.dtim > 0) --Hachi.dtim;
        if (Hachi.stim > 0) --Hachi.stim;
        execIns = 0;

        clk = clock();

    }

    atexit(cleanup);

    
    return 0;
}
