#include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "db.h"

DB_t *DB_init(DB_params_t params) {
	DB_t *db = malloc(sizeof(DB_t));

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
