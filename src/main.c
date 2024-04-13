#include "locale.h"
#include "ncurses.h"

#include "hachi.h"
#include "hachi.c"

void fetchIns();
void decodeIns();
void executeIns();

void setuphachi()
{
    // Copy Font data from FONT_TABLE to Hachi MEM

    int FontStAdr = 0x50;
    int FontEnAdr = 0x9F;
    int c = 0;

    for (int i=FontStAdr; i <= FontEnAdr; ++i) {
        Hachi.mem[i] = FONT_TABLE[c];
        ++c;
    }

}

int main(int argc, char **argv)
{

    setlocale(LC_ALL, "");

    setuphachi();

    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    while (!state.close) {

        fetchIns();
        decodeIns();
        executeIns();
    }


    return 0;
}
