#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

const int term_width = 80;
const int term_height = 24;
const int top_height = 6;
const int bottom_height = term_height - top_height - 1;

const char *DEFAULT_PREFIX = ".oOo. ";
const char *DEFAULT_SUFFIX = " .oOo.";

int print_title(char *title, const char *prefix, const char *suffix) {
	if (prefix == NULL) {
		prefix = DEFAULT_PREFIX;
	}

	if (suffix == NULL) {
		suffix = DEFAULT_SUFFIX;
	}

	size_t title_len = strlen(title);
	size_t prefix_len = strlen(prefix);
	size_t suffix_len = strlen(suffix);

	int x = (term_width - prefix_len - title_len - suffix_len) / 2;
	mvprintw(top_height, x, "%s%s%s", prefix, title, suffix);
	return 0;
}

int show_file(char *filename) {
	char *buffer = 0;
	long length;
	FILE *f = fopen (filename, "rb");

	if (f) {
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = malloc (length);
		if (buffer) {
			fread (buffer, 1, length, f);
		}
		fclose (f);
	}

	if (!buffer) {
		return 1;
	}

	int x = 0, y = 0, done = 0;
	long l = 0;
	while (!done) {
		if (x > term_width || buffer[l] == '\n') {
			if (buffer[l] == '\n') {
				l++;
			}
			x = 0;
			y++;

			if (y > top_height) {
				break;
			}
		}
		mvaddch(y, x++, buffer[l]);
		l++;
	}

	refresh();

	return 0;
}

int print_fkey_hint (int fn, const char* hint) {
	attron(A_BOLD);
	mvprintw(term_height - 1, 10*(fn - 1), "f%1d", fn);
	attroff(A_BOLD);
	attron(A_REVERSE);
	mvprintw(term_height - 1, 10*(fn - 1) + 2, "%-07.7s", hint);
	attroff(A_REVERSE);
	return 0;
}

int fkey_hints (
	const char *h1,
	const char *h2,
	const char *h3,
	const char *h4,
	const char *h5,
	const char *h6,
	const char *h7,
	const char *h8) {
	print_fkey_hint(1, h1);
	print_fkey_hint(2, h2);
	print_fkey_hint(3, h3);
	print_fkey_hint(4, h4);
	print_fkey_hint(5, h5);
	print_fkey_hint(6, h6);
	print_fkey_hint(7, h7);
	print_fkey_hint(8, h8);
	return 0;
}

