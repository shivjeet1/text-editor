#include <stdlib.h>

#include "../headers/editor.h"
#include "../headers/terminal.h"
#include "../headers/fileio.h"
#include "../headers/render.h"
#include "../headers/input.h"

struct editorConfig E;

void initEditor(void) {
	E.cx = 0;
	E.cy = 0;
	E.rx = 0;
	E.rowoff = 0;
	E.coloff = 0;
	E.numrows = 0;
	E.row = NULL;
	E.dirty = 0;
	E.filename = NULL;
	E.statusmsg[0] = '\0';
	E.statusmsg_time = 0;
	E.syntax = NULL;
    E.mode = MODE_NORMAL;
    E.sel_sx = 0;
    E.sel_sy = 0;
    E.sel_ex = 0;
    E.sel_ex = 0;

	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
		die("getWindowSize");

	E.screenrows -= 2;
}

int main(int argc, char *argv[]) {
	enableRawMode();
	initEditor();

	if (argc >= 2) {
		editorOpen(argv[1]);
	}

	editorSetStatusMessage(
		"HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");

	while (1) {
		editorRefreshScreen();
		editorProcessKeypress();
	}

	return 0;
}
