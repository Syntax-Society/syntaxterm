#ifndef DB_H
#define DB_H

#include <mariadb/mysql.h>

typedef struct {
	MYSQL *mysql;
} DB_t;

typedef struct {
	char *mysql_host;
	char *mysql_user;
	char *mysql_passwd;
	char *mysql_db;
	int mysql_port_num;
} DB_params_t;

DB_t *DB_init(DB_params_t);

int DB_term(DB_t *);

#endif
