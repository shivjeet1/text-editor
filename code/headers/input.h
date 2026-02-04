#ifndef INPUT_H
#define INPUT_H

#include "editor.h"

char *editorPrompt(char *prompt, void (*callback)(char *, int));

void editorMoveCursor(int key);
void editorProcessKeypress(void);

void editorInsertChar(int c);
void editorInsertNewline(void);
void editorDelChar(void);

#endif

