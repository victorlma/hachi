#ifndef NCURL_H_
#define NCURL_H_

#include "hachi.h"
typedef struct {
    b32  hascolor;
    const char* blockUnicode;
    int     h;
    int     w;
    int     centery;
    int     centerx;
    char ch;
    char charr[16];
    unsigned char cp;
} ncurInfo;

void ncur_clear();
void ncur_draw();
void ncur_end();
void ncur_handleInput();
void ncur_handleResize();
void ncur_setupkeys();
void ncur_setupscreen();

#endif
