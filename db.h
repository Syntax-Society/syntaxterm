#ifndef DB_H
#define DB_H

#include <mariadb/mysql.h>

typedef MYSQL* DB_t;

typedef struct {
	char *mysql_host;
	char *mysql_user;
	char *mysql_passwd;
	char *mysql_db;
	int mysql_port_num;
} DB_params_t;

typedef enum {
	DB_RET_OK,
	DB_RET_GENERAL_ERROR,
	DB_RET_USER_NOT_REGISTERED,
	DB_RET_USER_ALREADY_
} DB_return_code_t;

/**
 * init & term
 */
DB_t *DB_init(DB_params_t);
int DB_term(DB_t *);

/**
 * party_attendees
 */
typedef struct {
	char nickname[256];
	char party_title[256];
	char party_date[11];
} DB_party_attendee_pk_t;
int DB_set_party_attendee_status(DB_t *, DB_party_attendee_pk_t *, int);

/**
 * parties
 */
typedef struct {
	char title[256];
	char date[11];
} DB_party_pk_t;
int DB_get_next_party(DB_t *, DB_party_pk_t *);

#endif
