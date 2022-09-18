#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "context.h"
#include "common.h"

#define UTF8_STRING "UTF-8"
#define LATIN1_STRING "ISO_8859-1"

int errno;

Context_t *context_init (Context_params_t params, DB_t *db) {
	Context_t *context = calloc(1, sizeof(Context_t));
	params = params;

	context->latin1_to_utf8 = iconv_open(UTF8_STRING, LATIN1_STRING); // iconv_open(tocode, fromcode)
	if (context->latin1_to_utf8 == (iconv_t) -1) {
		perror("iconv_open from " LATIN1_STRING " to " UTF8_STRING " failed");
		return NULL;
	}

	context->utf8_to_latin1 = iconv_open(LATIN1_STRING, UTF8_STRING); // iconv_open(tocode, fromcode)
	if (context->utf8_to_latin1 == (iconv_t) -1) {
		perror("iconv_open from " UTF8_STRING " to " LATIN1_STRING " failed");
		return NULL;
	}

	context->db = db;

	context->banner = newwin(BANNER_HEIGHT, COLS, 0, 0);
	if (!context->banner) {
		perror("failed to create top window!");
		return NULL;
	}
	keypad(context->banner, TRUE);

	context->title = newwin(TITLE_HEIGHT, COLS, BANNER_HEIGHT, 0);
	if (!context->title) {
		perror("failed to create title window!");
		return NULL;
	}
	keypad(context->title, TRUE);

	context->main = newwin(LINES - (BANNER_HEIGHT + TITLE_HEIGHT + BOTTOM_HEIGHT), COLS, BANNER_HEIGHT + TITLE_HEIGHT, 0);
	if (!context->main) {
		perror("failed to create main window!");
		return NULL;
	}
	keypad(context->main, TRUE);

	context->bottom = newwin(BOTTOM_HEIGHT, COLS, LINES - BOTTOM_HEIGHT, 0);
	if (!context->bottom) {
		perror("failed to create bottom window!");
		return NULL;
	}
	keypad(context->bottom, TRUE);

	context->full = newwin(LINES, COLS, 0, 0);
	if (!context->full) {
		perror("failed to create full window!");
		return NULL;
	}
	keypad(context->full, TRUE);

	context->popup = newwin(LINES - 2, COLS/2, 1, COLS/4);
	if (!context->popup) {
		perror("failed to create popup window!");
		return NULL;
	}

	return context;
}

char *context_error() {
	return "asdf";
}

int context_term(Context_t *context) {
	if (context) {
		if (context->db) {
			DB_term(context->db);
		}

		if (context->utf8_to_latin1) {
			iconv_close(context->utf8_to_latin1);
		}

		if (context->latin1_to_utf8) {
			iconv_close(context->latin1_to_utf8);
		}

		if (context->banner) {
			delwin(context->banner);
		}

		if (context->title) {
			delwin(context->title);
		}

		if (context->main) {
			delwin(context->main);
		}

		if (context->bottom) {
			delwin(context->bottom);
		}

		if (context->full) {
			delwin(context->full);
		}

		free(context);
	}

	return 0;
}
