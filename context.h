#ifndef CONTEXT_H
#define CONTEXT_H

#include <iconv.h>
#include <ncurses.h>
#include <mariadb/mysql.h>
#include "db.h"

typedef struct {
	iconv_t latin1_to_utf8;
	iconv_t utf8_to_latin1;
	DB_t *db;
	MYSQL *mysql;
	WINDOW *banner;
	WINDOW *title;
	WINDOW *main;
	WINDOW *bottom;
	WINDOW *full;
	WINDOW *popup;
} Context_t;

typedef struct {
} Context_params_t;

Context_t *context_init(Context_params_t, DB_t *);
char *context_error();
int context_term(Context_t *);

#endif
