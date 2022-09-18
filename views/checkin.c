#include <curses.h>
#include <string.h>
#include <form.h>
#include <mariadb/mysql.h>
#include "common.h"
#include "context.h"
#include "views/checkin.h"

// based on https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/forms.html
// and https://gist.github.com/alan-mushi/c8a6f34d1df18574f643

const char *suggestions_query = "select nickname, count(*) from party_attendees where nickname like '%%%s%%' group by nickname order by count(*) desc limit %d";
const char *checkin_query = "update party_attendees set attended=1 where nickname='%s' and ";
const char *nickname_prompt = "Nickname: ";
const int NUM_SUGGESTIONS = 5;
char suggestions[5][256];
const int FORM_LEFT = 1, FORM_TOP = 1;

int get_suggestions (Context_t *context, char *str) {
	char query[256];
	snprintf(query, 256, suggestions_query, str, NUM_SUGGESTIONS);

	mysql_query(context->db->mysql, query);
	MYSQL_RES *result = mysql_store_result(context->db->mysql);
	MYSQL_ROW row;
	int i = 0;
	while ((row = mysql_fetch_row(result))) {
		strncpy(suggestions[i], row[0], 256);
		i++;
	}
	for (; i < 5; i++) {
		suggestions[i][0] = '\0';
	}
	mysql_free_result(result);
	return 0;
}

int update_suggestions (Context_t *context) {
	int y, x;
	getyx(context->main, y, x); // save cursor position

	int top = 3;
	for (int i = 0; i < 5; i++) {
		mvwprintw(context->main, top + i, 1, "f%d:                        ", i+1);
		mvwprintw(context->main, top + i, 1+4, "%.8s", suggestions[i]);
	}

	wmove(context->main, y, x); // restore cursor position
	return 0;
}

int update_checked_in (void) {
	return 0;
}

int check_in(Context_t *context, DB_party_pk_t *next_party, int suggestion_number) {
	if (strlen(suggestions[suggestion_number]) == 0) {
		return 1;
	}

	DB_party_attendee_pk_t attendee;
	strncpy(attendee.nickname, suggestions[suggestion_number], sizeof(attendee.nickname));
	strncpy(attendee.party_title, next_party->title, sizeof(attendee.party_title));
	strncpy(attendee.party_date, next_party->date, sizeof(attendee.party_date));
	// try to check in
	switch (DB_set_party_attendee_status(context->db, &attendee, 1)) {
		case 0:
			mvwprintw(context->main, 12, 10, "besokare %s anmäld till %s!", attendee.nickname, attendee.party_title);
			break;
		case 1:
			mvwprintw(context->main, 12, 10, "besokare %s ej anmald till %s!", attendee.nickname, attendee.party_title);
			break;
		case 2:
			mvwprintw(context->main, 12, 10, "besokare %s redan incheckad till %s!", attendee.nickname, attendee.party_title);
			break;
		case 3:
			mvwprintw(context->main, 12, 10, "could not set as attending: %s", mysql_error(context->db->mysql));
			break;
	}
	return 0;
}

int checkin (Context_t *context) {
	FIELD *fields[2];
	fields[0] = new_field(1, 10, 0, 0, 0, 0);
	if (!fields[0]) {
		perror("could not create field");
		return 1;
	}
	fields[1] = NULL;
	set_field_back(fields[0], A_UNDERLINE);
	field_opts_off(fields[0], O_AUTOSKIP);

	FORM *checkin_form = new_form(fields);

	int rows, cols;
	scale_form(checkin_form, &rows, &cols);

	WINDOW *form_window = derwin(context->main, rows, cols, 1, 2);
	set_form_win(checkin_form, context->main);
	set_form_sub(checkin_form, form_window);

	box(form_window, 0, 0);

	post_form(checkin_form);
	wrefresh(context->main);

	print_title(context, "Incheckning", NULL, NULL);

	DB_party_pk_t next_party;
	DB_get_next_party(context->db, &next_party);

	fkey_hints(
		context,
		"",
		"",
		"",
		"",
		"",
		"anmal",
		"",
		"bakat"
	);

	mvwaddstr(context->main, 0, 1, nickname_prompt);
	mvwaddch(context->main, 1, 1, '>');
	wmove(context->main, 1, 2);

	get_suggestions(context, "");
	update_suggestions(context);

	int c;
	while ((c = wgetch(context->main)) != KEY_F(8)) {
		switch (c) {
			FORM_UX_CASES(checkin_form)
			case KEY_F(1):
				check_in(context, &next_party, 0);
				break;
			case KEY_F(2):
				check_in(context, &next_party, 1);
				break;
			case KEY_F(3):
				check_in(context, &next_party, 2);
				break;
			case KEY_F(4):
				check_in(context, &next_party, 3);
				break;
			case KEY_F(5):
				check_in(context, &next_party, 4);
				break;
		}
		form_driver(checkin_form, REQ_VALIDATION);

		get_suggestions(context, trim_whitespaces(field_buffer(fields[0], 0)));
		update_suggestions(context);

		wrefresh(context->main);
	}
	wrefresh(context->main);

	unpost_form(checkin_form);
	free_form(checkin_form);
	free_field(fields[0]);

	return 0;
}
