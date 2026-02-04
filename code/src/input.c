#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "../headers/editor.h"
#include "../headers/input.h"
#include "../headers/row.h"
#include "../headers/render.h"
#include "../headers/terminal.h"
#include "../headers/fileio.h"
#include "../headers/search.h"

/*** editor operations ***/

void editorInsertChar(int c) {
	if (E.cy == E.numrows) {
		editorInsertRow(E.numrows, "", 0);
	}
	editorRowInsertChar(&E.row[E.cy], E.cx, c);
	E.cx++;
}

void editorInsertNewline(void) {
	if (E.cx == 0) {
		editorInsertRow(E.cy, "", 0);
	} else {
		erow *row = &E.row[E.cy];
		editorInsertRow(E.cy + 1,
		                &row->chars[E.cx],
		                row->size - E.cx);
		row = &E.row[E.cy];
		row->size = E.cx;
		row->chars[row->size] = '\0';
		editorUpdateRow(row);
	}
	E.cy++;
	E.cx = 0;
}

void editorDelChar(void) {
	if (E.cy == E.numrows) return;
	if (E.cx == 0 && E.cy == 0) return;

	erow *row = &E.row[E.cy];
	if (E.cx > 0) {
		editorRowDelChar(row, E.cx - 1);
		E.cx--;
	} else {
		E.cx = E.row[E.cy - 1].size;
		editorRowAppendString(&E.row[E.cy - 1],
		                      row->chars,
		                      row->size);
		editorDelRow(E.cy);
		E.cy--;
	}
}

/*** prompt ***/

char *editorPrompt(char *prompt, void (*callback)(char *, int)) {
	size_t bufsize = 128;
	char *buf = malloc(bufsize);

	size_t buflen = 0;
	buf[0] = '\0';

	while (1) {
		editorSetStatusMessage(prompt, buf);
		editorRefreshScreen();

		int c = editorReadKey();
		if (c == BACKSPACE || c == CTRL_KEY('h') || c == DEL_KEY) {
			if (buflen != 0)
				buf[--buflen] = '\0';
		} else if (c == '\x1b') {
			editorSetStatusMessage("");
			if (callback) callback(buf, c);
			free(buf);
			return NULL;
		} else if (c == '\r') {
			if (buflen != 0) {
				editorSetStatusMessage("");
				if (callback) callback(buf, c);
				return buf;
			}
		} else if (!iscntrl(c) && c < 128) {
			if (buflen == bufsize - 1) {
				bufsize *= 2;
				buf = realloc(buf, bufsize);
			}
			buf[buflen++] = c;
			buf[buflen] = '\0';
		}

		if (callback)
			callback(buf, c);
	}
}

/*** cursor movement ***/

void editorMoveCursor(int key) {
	erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

	switch (key) {
		case ARROW_LEFT:
			if (E.cx != 0) {
				E.cx--;
			} else if (E.cy > 0) {
				E.cy--;
				E.cx = E.row[E.cy].size;
			}
			break;

		case ARROW_RIGHT:
			if (row && E.cx < row->size) {
				E.cx++;
			} else if (row && E.cx == row->size) {
				E.cy++;
				E.cx = 0;
			}
			break;

		case ARROW_UP:
			if (E.cy != 0)
				E.cy--;
			break;

		case ARROW_DOWN:
			if (E.cy < E.numrows)
				E.cy++;
			break;
	}

	row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
	int rowlen = row ? row->size : 0;
	if (E.cx > rowlen)
		E.cx = rowlen;

    if (E.mode == MODE_VISUAL) {
        E.sel_ex = E.cx;
        E.sel_ey = E.cy;
    }
}

/*** key processing ***/

void editorProcessKeypress(void) {
	static int quit_times = JADU_QUIT_TIMES;

	int c = editorReadKey();

	switch (c) {
		case '\r':
			editorInsertNewline();
			break;

		case CTRL_KEY('q'):
			if (E.dirty && quit_times > 0) {
				editorSetStatusMessage(
					"WARNING!!! File has unsaved changes! "
					"Press Ctrl-Q %d more times to quit",
					quit_times);
				quit_times--;
				return;
			}
			write(STDOUT_FILENO, "\x1b[2J", 4);
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;

		case CTRL_KEY('s'):
			editorSave();
			break;

		case CTRL_KEY('f'):
			editorFind();
			break;

        case CTRL_KEY('v'):
            if (E.mode == MODE_NORMAL) {
                E.mode = MODE_VISUAL;
                E.sel_sx = E.cx;
                E.sel_sy = E.cy;
                E.sel_ex = E.cx;
                E.sel_ey = E.cy;
            }
            break;

        case ('\x1b' || CTRL_KEY('c')):
            if (E.mode == MODE_VISUAL) {
                E.mode = MODE_NORMAL;
            }
            break;

		case HOME_KEY:
			E.cx = 0;
			break;

		case END_KEY:
			if (E.cy < E.numrows)
				E.cx = E.row[E.cy].size;
			break;

		case BACKSPACE:
		case CTRL_KEY('h'):
		case DEL_KEY:
			if (c == DEL_KEY)
				editorMoveCursor(ARROW_RIGHT);
			editorDelChar();
			break;

		case PAGE_UP:
		case PAGE_DOWN: {
			if (c == PAGE_UP)
				E.cy = E.rowoff;
			else {
				E.cy = E.rowoff + E.screenrows - 1;
				if (E.cy > E.numrows)
					E.cy = E.numrows;
			}

			int times = E.screenrows;
			while (times--)
				editorMoveCursor(
					c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
		} break;

		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
			editorMoveCursor(c);
			break;

		case CTRL_KEY('l'):
		case '\x1b':
			break;

		default:
			editorInsertChar(c);
			break;
	}

	quit_times = JADU_QUIT_TIMES;
}
