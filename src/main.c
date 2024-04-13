#include "locale.h"
#include "ncurses.h"

#include "hachi.h"
#include "ncur_layer.h"
#include "x11_layer.h"
#include "hachi.c"

int main(int argc, char **argv)
{

    setlocale(LC_ALL, "");

    setuphachi();


    while (!Hachi.close) {

        fetchIns();
        decodeIns();
        executeIns();
    }


    return 0;
}
