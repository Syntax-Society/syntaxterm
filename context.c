#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include "context.h"

#define UTF8_STRING "UTF-8"
#define LATIN1_STRING "ISO_8859-1"

int errno;

Context_t *context_init (Context_params_t params) {
	Context_t *context = malloc(sizeof(Context_t));

	context->latin1_to_utf8 = iconv_open(UTF8_STRING, LATIN1_STRING); // iconv_open(tocode, fromcode)
	if (context->latin1_to_utf8 == (iconv_t) -1) {
		perror("iconv_open from " LATIN1_STRING " to " UTF8_STRING " failed");
		return NULL;
	}

	context->utf8_to_latin1 = iconv_open(LATIN1_STRING, UTF8_STRING); // iconv_open(tocode, fromcode)
	if (context->utf8_to_latin1 == (iconv_t) -1) {
		perror("iconv_open from " UTF8_STRING " to " LATIN1_STRING " failed");
		return NULL;
	}

	context->db = DB_init(
		(DB_params_t) {
			params.mysql_host,
			params.mysql_user,
			params.mysql_passwd,
			params.mysql_db,
			params.mysql_port
		}
	);
	if (context->db == NULL) {
		fprintf(stderr, "Could not initialize database!\n");
		return NULL;
	}

	return context;
}

char *context_error() {
	return "asdf";
}

int context_term(Context_t *context) {
	if (context) {
		if (context->db) {
			DB_term(context->db);
		}

		if (context->utf8_to_latin1) {
			iconv_close(context->utf8_to_latin1);
		}

		if (context->latin1_to_utf8) {
			iconv_close(context->latin1_to_utf8);
		}

		free(context);
	}

	return 0;
}
