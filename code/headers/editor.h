#ifndef EDITOR_H
#define EDITOR_H

#include <termios.h>
#include <time.h>
#include <stddef.h>

#define JADU_VERSION "0.0.1"
#define JADU_TAB_STOP 8
#define JADU_QUIT_TIMES 3

#define CTRL_KEY(k) ((k) & 0x1f)

enum editorKey {
	BACKSPACE = 127,
	ARROW_LEFT = 1000,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN,
	PAGE_UP,
	PAGE_DOWN,
	HOME_KEY,
	END_KEY,
	DEL_KEY
};

enum editorHighlight {
	HL_NORMAL = 0,
	HL_STRING,
	HL_NUMBER,
	HL_MLCOMMENT,
	HL_MATCH,
	HL_COMMENT,
	HL_KEYWORD1,
	HL_KEYWORD2
};

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

struct editorSyntax {
	char *filetype;
	char **filematch;
	char *singleline_comment_start;
	char *multiline_comment_start;
	char *multiline_comment_end;
	char **keywords;
	int flags;
};

typedef struct erow {
	int idx;
	int size;
	int rsize;
	char *chars;
	char *render;
	unsigned char *hl;
	int hl_open_comment;
} erow;

struct editorConfig {
	int cx, cy;
	int rx;
	int rowoff;
	int coloff;
	int screenrows;
	int screencols;
	int numrows;
	erow *row;
	int dirty;
	char *filename;
	char statusmsg[80];
	time_t statusmsg_time;
	struct editorSyntax *syntax;
	struct termios orig_termios;
};

extern struct editorConfig E;

void die(const char *s);
void enableRawMode(void);
void disableRawMode(void);
int editorReadKey(void);
int getWindowSize(int *rows, int *cols);

void editorUpdateSyntax(erow *row);
int editorSyntaxToColor(int hl);
void editorSelectSyntaxHighlight(void);

int editorRowCxToRx(erow *row, int cx);
int editorRowRxToCx(erow *row, int rx);
void editorUpdateRow(erow *row);
void editorInsertRow(int at, char *s, size_t len);
void editorDelRow(int at);
void editorRowInsertChar(erow *row, int at, int c);
void editorRowAppendString(erow *row, char *s, size_t len);
void editorRowDelChar(erow *row, int at);
void editorFreeRow(erow *row);

void editorInsertChar(int c);
void editorInsertNewline(void);
void editorDelChar(void);

void editorOpen(char *filename);
void editorSave(void);

void editorFind(void);

void editorRefreshScreen(void);
void editorSetStatusMessage(const char *fmt, ...);

char *editorPrompt(char *prompt, void (*callback)(char *, int));
void editorMoveCursor(int key);
void editorProcessKeypress(void);

void initEditor(void);

#endif

