#ifndef FILEIO_H
#define FILEIO_H

#include <stddef.h>
#include "editor.h"

void editorOpen(char *filename);
void editorSave(void);

char *editorRowsToString(int *buflen);

#endif 

