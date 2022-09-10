#ifndef COMMON_H
#define COMMON_H

#include <curses.h>
#include <iconv.h>

/*
 * Macros
 */
#define FORM_UX_CASES(form) \
case KEY_LEFT:\
	form_driver(form, REQ_PREV_CHAR);\
	break;\
case KEY_RIGHT:\
	form_driver(form, REQ_NEXT_CHAR);\
	break;\
case KEY_UP:\
	form_driver(form, REQ_PREV_FIELD);\
	form_driver(form, REQ_END_LINE);\
	break;\
case KEY_DOWN:\
	form_driver(form, REQ_NEXT_FIELD);\
	form_driver(form, REQ_END_LINE);\
	break;\
case KEY_HOME:\
	form_driver(form, REQ_BEG_LINE);\
	break;\
case KEY_END:\
	form_driver(form, REQ_END_LINE);\
	break;\
case KEY_BACKSPACE:\
case 127:\
	form_driver(form, REQ_DEL_PREV);\
	break;\
case KEY_DC:\
	form_driver(form, REQ_DEL_CHAR);\
	break;\
default:\
	form_driver(checkin_form, c);\
	break;

/*
 * Constants
 */
extern const int term_width;
extern const int term_height;
extern const int top_height;
extern const int bottom_height;

/*
 * Functions
 */
int print_title(char *, const char *, const char *);
int show_file(char *);
int fkey_hints (
	const char *,
	const char *,
	const char *,
	const char *,
	const char *,
	const char *,
	const char *,
	const char *);

#endif
