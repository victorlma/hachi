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
    
    

    float fps = 60;
    float target = 1.0f/fps;

    struct timespec tspec = {0};

    setuphachi();


    clock_t clk = clock();
    while (!Hachi.close) {


        handleInput();
        handleResize();
        decodeAndExec(fetchIns());
        execIns++;


        clk = clock() - clk;

        float ms = (float) clk/CLOCKS_PER_SEC;
        
        if (ms < target && execIns >= Hachi.insPerFrame)
        {
            tspec.tv_sec = 0;
            tspec.tv_nsec = (target - ms) * 1000000000L;
            execIns = 0;

            nanosleep(&tspec, &tspec);
        }


        clk = clock();

    }

    atexit(cleanup);

    
    return 0;
}
