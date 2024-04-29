#include "locale.h"
#include "stdio.h"
#include "stdlib.h"
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

    int offset = 0;
    for (int i=0; i < filesize; ++i)
    {
        unsigned char bf[2];
        fread(bf, sizeof(unsigned char), 2, fp);
        Hachi.mem[512+offset++] = bf[0];
        Hachi.mem[512+offset++] = bf[1];
        

    }
    
    

    setuphachi();


    while (!Hachi.close) {

        decodeAndExec(fetchIns());
    }

    atexit(cleanup);

    
    return 0;
}
