#ifndef CONFIG_H
#define CONFIG_H


struct Config_s {
	char *mysql_host;
	char *mysql_user;
	char *mysql_passwd;
	char *mysql_db;
	int mysql_port;
} config = {
	"127.0.0.1",
	"pi",
	"hallonfastpaengelska",
	"syntaxsociety",
	3306,
};

#endif
