#include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h> // XXX
#include "db.h"

/**
 * queries
 */
const char *get_next_party_query = "select title, start_date from parties where end_date >= curdate() order by start_date limit 1";
const char *get_party_attendee_registered_query = "select attended from party_attendees where nickname='%s' and party_title='%s' and party_date='%s'";
const char *set_party_attendee_status_query = "update party_attendees set attended=%d where nickname='%s' and party_title='%s' and party_date='%s'";


/**
 * party_attendees
 */
int DB_set_party_attendee_status(DB_t *db, DB_party_attendee_pk_t *attendee, int status) {
	char query[256];
	snprintf(
		query,
		sizeof(query),
		get_party_attendee_registered_query,
		attendee->nickname,
		attendee->party_title,
		attendee->party_date
	);
	if (mysql_query(db->mysql, query)) {
		return 3; // general error
	}
	MYSQL_RES *result = mysql_store_result(db->mysql);
	if (mysql_num_rows(result) < 1) {
		mysql_free_result(result);
		return 1; // user not registered
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	if (atoi(row[0]) == status) {
		mysql_free_result(result);
		return 2; // status is already desired
	}
	mysql_free_result(result);

	memset(query, '\0', sizeof(query));
	snprintf(
		query,
		sizeof(query),
		set_party_attendee_status_query,
		status,
		attendee->nickname,
		attendee->party_title,
		attendee->party_date
	);
	mvprintw(30, 10, "%s", query);
	//refresh();
	if (mysql_query(db->mysql, query)) {
		return 3; // general error
	}
	return 0; // success
}

/**
 * parties
 */
int DB_get_next_party(DB_t *db, DB_party_pk_t *next_party) {
	mysql_query(db->mysql, get_next_party_query);

	MYSQL_RES *result = mysql_store_result(db->mysql);
	if (!result) {
		return 2;
	}

	MYSQL_ROW row = mysql_fetch_row(result);
	if (!row) {
		return 1;
	}

	strncpy(next_party->title, row[0], 256);
	strncpy(next_party->date, row[1], 11);

	return 0;
}

DB_t *DB_init(DB_params_t params) {
	DB_t *db = calloc(1, sizeof(DB_t));

	if (mysql_library_init(0, NULL, NULL)) { // only needed in multithreaded applications, but meh
		fprintf(stderr, "could not initialize MySQL client library\n");
		return NULL;
	}

	db->mysql = mysql_init(NULL); // NULL to create a new object
	if (db->mysql == NULL) {
		fprintf(stderr, "Could not create MySQL object: %s\n", mysql_error(db->mysql));
		return NULL;
	}

	if (mysql_real_connect(db->mysql, params.mysql_host, params.mysql_user, params.mysql_passwd, params.mysql_db, params.mysql_port_num, NULL, 0) == NULL) {
		fprintf(stderr, "Could not connect to the MySQL database: %s\n", mysql_error(db->mysql));
		return NULL;
	}

	printf("Connected to MySQL server version %s connected at %s\nStatus: %s\n",
		mysql_get_server_info(db->mysql),
		mysql_get_host_info(db->mysql),
		mysql_stat(db->mysql)
	);

	MYSQL_RES *result = mysql_list_dbs(db->mysql, NULL);
	if (result == NULL) {
		fprintf(stderr, "Could not list databases!\n");
	} else {
		printf("Databases:\n");
		MYSQL_ROW row;
		int n = 0;
		while ((row = mysql_fetch_row(result))) {
			for (unsigned int i = 0; i < mysql_num_fields(result); i++) {
				printf("%d: %s", n++, row[i]);
			}
			printf("\n");
		}
		mysql_free_result(result);
	}

	return db;
}

int DB_term(DB_t *db) {
	if (db) {
		mysql_close(db->mysql);
		free(db);
	}

	mysql_library_end();

	return 0;
}
