#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>
#include "common.h"
#include "context.h"

const int BANNER_HEIGHT = 6;
const int TITLE_HEIGHT = 1;
const int BOTTOM_HEIGHT = 1;
const int MAIN_HEIGHT = 24 - BANNER_HEIGHT - TITLE_HEIGHT - BOTTOM_HEIGHT;

const char *DEFAULT_PREFIX = ".oOo. ";
const char *DEFAULT_SUFFIX = " .oOo.";

size_t num_files_in_dir (const char *dirname) {
	size_t num = 0;
	DIR *d = opendir(dirname);
	struct dirent *dir;
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG) {
				num++;
			}
		}
		closedir(d);
	}
	return num;
}

char *trim_whitespaces(char *str) {
	char *end;

	// trim leading space
	while (isspace(*str)) {
		str++;
	}

	if (*str == 0) { // all spaces?
		return str;
	}

	// trim trailing space
	end = str + strnlen(str, 128) - 1;

	while (end > str && isspace(*end)) {
		end--;
	}

	// write new null terminator
	*(end+1) = '\0';

	return str;
}

int print_title(Context_t *ctx, char *title, const char *prefix, const char *suffix) {
	if (prefix == NULL) {
		prefix = DEFAULT_PREFIX;
	}

	if (suffix == NULL) {
		suffix = DEFAULT_SUFFIX;
	}

	size_t title_len = strlen(title);
	size_t prefix_len = strlen(prefix);
	size_t suffix_len = strlen(suffix);

	int x = (COLS - prefix_len - title_len - suffix_len) / 2;
	mvwprintw(ctx->title, 0, x, "%s%s%s", prefix, title, suffix);
	wrefresh(ctx->title);
	return 0;
}

int show_banner (Context_t *ctx) {
	size_t num_files = num_files_in_dir("banners/");
	int file_num_used = rand() % num_files;
	DIR *d = opendir("banners/");
	struct dirent *dir;
	if (d) {
		int n = 0;
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG) {
				if (n++ == file_num_used) {
					break;
				}
			}
		}
		closedir(d);

		char filename[9+NAME_MAX];
		snprintf(filename, sizeof(filename), "banners/%s", dir->d_name);

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
			if (x > COLS || buffer[l] == '\n') {
				if (buffer[l] == '\n') {
					l++;
				}
				x = 0;
				y++;

				if (y > BANNER_HEIGHT) {
					break;
				}
			}
			if (buffer[l] == 0) {
				break;
			}
			mvwaddch(ctx->banner, y, x++, buffer[l]);
			l++;
		}

		wrefresh(ctx->banner);
	}

	return 0;
}

int print_fkey_hint (Context_t *ctx, int fn, const char* hint) {
	wattron(ctx->bottom, A_BOLD);
	mvwprintw(ctx->bottom, 0, 10*(fn - 1), "f%1d", fn);
	wattroff(ctx->bottom, A_BOLD);
	wattron(ctx->bottom, A_REVERSE);
	mvwprintw(ctx->bottom, 0, 10*(fn - 1) + 2, "%-7.7s", hint);
	wattroff(ctx->bottom, A_REVERSE);
	return 0;
}

int fkey_hints (
	Context_t *ctx,
	const char *h1,
	const char *h2,
	const char *h3,
	const char *h4,
	const char *h5,
	const char *h6,
	const char *h7,
	const char *h8) {
	print_fkey_hint(ctx, 1, h1);
	print_fkey_hint(ctx, 2, h2);
	print_fkey_hint(ctx, 3, h3);
	print_fkey_hint(ctx, 4, h4);
	print_fkey_hint(ctx, 5, h5);
	print_fkey_hint(ctx, 6, h6);
	print_fkey_hint(ctx, 7, h7);
	print_fkey_hint(ctx, 8, h8);
	wrefresh(ctx->bottom);
	return 0;
}

