#ifndef CONTEXT_H
#define CONTEXT_H

#include <iconv.h>
#include "db.h"

typedef struct {
	iconv_t latin1_to_utf8;
	iconv_t utf8_to_latin1;
	DB_t *db;
} Context_t;

typedef struct {
	char *mysql_host;
	char *mysql_user;
	char *mysql_passwd;
	char *mysql_db;
	int mysql_port;
} Context_params_t;

Context_t *context_init();
char *context_error();
int context_term(Context_t *);

#endif
