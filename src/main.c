#include "locale.h"
#include "ncurses.h"


typedef int b32;

char FONT_TABLE[16*5] = {
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


typedef struct
{
    b32     close;

    char    mem[4096];  // 4kb memory:  4k = 4096
    char    dpy[8*4];   // The display is 64 pixels wide and 32 pixels tall. Each pixel can be on or off. In other words, each pixel is a boolean value, or a bit. 64bit = 8 bytes
    char    dtim;       // 8bit delay timer
    char    stim;       // 8bit sound timer
    char    vreg[16];   // 15 Variable Registers
    
    short int ireg;     // 16-bit index register called “I” which is used to point at locations in memory
    short int *instack; // A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
    char *pc;           // A program counter, often called just “PC”, which points at the current instruction in memory

} hachiChip;

hachiChip  Hachi = {0};

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
