#ifndef HACHI_H_
#define HACHI_H_

#define dpy_w 64
#define dpy_h 32
#define dpy_wb dpy_w/8
#define dpy_hb dpy_h/4

typedef int b32;

typedef enum { NCUR, X11_GL, BACKEND_COUNT} HACHI_BACKEND;
typedef enum { CHIP8, CHIP48, SUPERCHIP, QUIRK_COUNT} HACHI_QUIRK;


typedef struct
{
    unsigned char indx;
    unsigned char bitmask;

} dpyindex;

typedef struct
{
    unsigned long key;
    b32 state;
} hachiBtn;

typedef struct
{
    hachiBtn  b0;
    hachiBtn  b1;
    hachiBtn  b2;
    hachiBtn  b3;
    hachiBtn  b4;
    hachiBtn  b5;
    hachiBtn  b6;
    hachiBtn  b7;
    hachiBtn  b8;
    hachiBtn  b9;
    hachiBtn  ba;
    hachiBtn  bb;
    hachiBtn  bc;
    hachiBtn  bd;
    hachiBtn  be;
    hachiBtn  bf;

} hachiKeypad;

typedef struct
{
    HACHI_BACKEND  backend;
    HACHI_QUIRK  quirk;

    b32     close;

    unsigned char    mem[4096];  // 4kb memory:  4k = 4096
    unsigned char    dpy[dpy_wb * dpy_h];   // The display is 64 pixels wide and 32 pixels tall. Each pixel can be on or off. In other words, each pixel is a boolean value, or a bit. 64bit = 8 bytes
    unsigned char    dtim;       // 8bit delay timer
    unsigned char    stim;       // 8bit sound timer
    unsigned char    vreg[16];   // 15 Variable Registers
    
    unsigned short int ireg;     // 16-bit index register called “I” which is used to point at locations in memory
    unsigned short int stack[2];
    unsigned char  stackCursor; // A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
    int  pc;           // A program counter, often called just “PC”, which points at the current instruction in memory

    hachiKeypad  k;

} hachiChip;

void setuphachi();

unsigned short int fetchIns();
void decodeAndExec(unsigned short int ins);


void clearscreen();
void drawscreen();
void setupscreen();



#endif
