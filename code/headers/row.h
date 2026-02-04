#ifndef ROW_H
#define ROW_H

#include <stddef.h>
#include "editor.h"

int editorRowCxToRx(erow *row, int cx);
int editorRowRxToCx(erow *row, int rx);

void editorUpdateRow(erow *row);

void editorInsertRow(int at, char *s, size_t len);
void editorDelRow(int at);
void editorFreeRow(erow *row);

void editorRowInsertChar(erow *row, int at, int c);
void editorRowAppendString(erow *row, char *s, size_t len);
void editorRowDelChar(erow *row, int at);

#endif 

