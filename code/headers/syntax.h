#ifndef SYNTAX_H
#define SYNTAX_H

#include "editor.h"

void editorUpdateSyntax(erow *row);
int editorSyntaxToColor(int hl);
void editorSelectSyntaxHighlight(void);

#endif 

