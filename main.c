#include <curses.h>
#include <form.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "config.h"
#include "context.h"
#include "db.h"
#include "views/main_menu.h"

Context_t *context;

static void finish(int sig) {
	sig = sig;
	endwin();
	context_term(context);

	printf("Exited gracefully!\n");
	exit(0);
}

int main(void) {
	srand(time(NULL));

	DB_t *db = DB_init(
		(DB_params_t) {
			config.mysql_host,
			config.mysql_user,
			config.mysql_passwd,
			config.mysql_db,
			config.mysql_port
		}
	);
	if (db == NULL) {
		fprintf(stderr, "Could not initialize database!\n");
		return 1;
	}

	signal(SIGINT, finish);
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	context = context_init((Context_params_t) {
	}, db);
	if (context == NULL) {
		fprintf(stderr, "Could not initialize context: %s\n", context_error());
		return 2;
	}

	set_form_win(NULL, context->main);
	main_menu(context);

	finish(0);

	return 0;
}
