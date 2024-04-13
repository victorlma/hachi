#ifndef HACHI_H_
#define HACHI_H_

typedef int b32;


typedef struct
{
    unsigned long key;
    b32 state;
} hachiBtn;

typedef struct
{
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
    b32     close;

    char    mem[4096];  // 4kb memory:  4k = 4096
    char    dpy[8*4];   // The display is 64 pixels wide and 32 pixels tall. Each pixel can be on or off. In other words, each pixel is a boolean value, or a bit. 64bit = 8 bytes
    char    dtim;       // 8bit delay timer
    char    stim;       // 8bit sound timer
    char    vreg[16];   // 15 Variable Registers
    
    short int ireg;     // 16-bit index register called “I” which is used to point at locations in memory
    short int *instack; // A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
    char *pc;           // A program counter, often called just “PC”, which points at the current instruction in memory

    hachiKeypad  k;

} hachiChip;

void setuphachi();

#endif
