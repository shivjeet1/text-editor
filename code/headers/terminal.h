#ifndef TERMINAL_H
#define TERMINAL_H

#include "editor.h"

void die(const char *s);
void enableRawMode(void);
void disableRawMode(void);

int editorReadKey(void);

int getWindowSize(int *rows, int *cols);

#endif 

