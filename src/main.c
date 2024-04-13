#include "locale.h"
#include "ncurses.h"

#include "hachi.h"
#include "hachi.c"

void fetchIns();
void decodeIns();
void executeIns();


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
