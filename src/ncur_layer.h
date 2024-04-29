#ifndef NCURL_H_
#define NCURL_H_

#include "hachi.h"
typedef struct {
    b32  hascolor;
    const char* blockUnicode;
} ncurInfo;

void ncur_setupkeys();
void ncur_setupscreen();
void ncur_clear();
void ncur_draw();

#endif
