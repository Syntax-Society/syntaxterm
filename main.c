#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "config.h"
#include "context.h"
#include "main_menu.h"

Context_t *context;

static void finish(int sig) {
	sig = sig;

	endwin();

	context_term(context);

	exit(0);
}

int main(void) {
	context = context_init(
		(Context_params_t) {
			config.mysql_host,
			config.mysql_user,
			config.mysql_passwd,
			config.mysql_db,
			config.mysql_port
		}
	);

	if (context == NULL) {
		fprintf(stderr, "Could not initialize context: %s\n", context_error());
		return 1;
	}

	signal(SIGINT, finish);
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	for (int x = 0; x < 80; x++) {
		mvaddch(24, x, '0' + x % 10);
	}
	for (int y = 0; y < 24; y++) {
		mvaddch(y, 80, '0' + y % 10);
	}
	main_menu(context);

	finish(0);

	return 0;
}
