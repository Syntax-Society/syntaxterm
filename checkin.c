#include <curses.h>
#include <form.h>
#include <mariadb/mysql.h>
#include "common.h"
#include "context.h"
#include "checkin.h"

// based on https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/forms.html
// and https://gist.github.com/alan-mushi/c8a6f34d1df18574f643

int checkin(Context_t *context) {
	char query[256];
	FIELD *fields[2];
	fields[0] = new_field(1, 10, top_height + 2, 18, 0, 0);
	if (!fields[0]) {
		perror("could not create field");
		return 1;
	}
	fields[1] = NULL;
	set_field_back(fields[0], A_UNDERLINE);
	field_opts_off(fields[0], O_AUTOSKIP);

	FORM *checkin_form = new_form(fields);
	post_form(checkin_form);
	refresh();

	print_title("Incheckning", NULL, NULL);
	//show_file("header.txt");
	fkey_hints(
		"checka in",
		"",
		"",
		"",
		"",
		"",
		"",
		"bakat"
	);

	mvprintw(top_height + 2, 10, "Value 1:");
	refresh();

	int c;
	while ((c = wgetch(stdscr)) != KEY_F(8)) {
		switch (c) {
			FORM_UX_CASES(checkin_form)
			case KEY_F(1):
				// try to check in
				break;
		}
		int y, x;
		getyx(stdscr, y, x);
		form_driver(checkin_form, REQ_VALIDATION);

		snprintf(query, 256, "select * from members where nickname like %s%%", field_buffer(fields[0], 0));
		mysql_query(context->db->mysql, query);
		MYSQL_RES *result = mysql_store_result(context->db->mysql);
		MYSQL_ROW row;
		int i = 0;
		size_t num_fields = mysql_num_fields(result);
		while((row = mysql_fetch_row(result))) {
			unsigned long *lengths = mysql_fetch_lengths(result);
			for (size_t j = 0; j < num_fields; j++) {
				mvprintw(top_height + 4 + i, 10 + 10 * j, "[%.*s]", (int) lengths[j], row[j] ? row[j] : NULL);
			}
			i++;
		}
		mysql_free_result(result);

		// field_buffer(fields[0], 0) = input.value
		mvprintw(top_height + 3, 10, ">%s<", field_buffer(fields[0], 0));
		move(y, x);
		refresh();
	}
	refresh();

	unpost_form(checkin_form);
	free_form(checkin_form);
	free_field(fields[0]);

	return 0;
}
