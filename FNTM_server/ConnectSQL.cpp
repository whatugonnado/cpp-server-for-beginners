#include "ConnectSQL.h"
#include "setting.h"

#include <iostream>

ConnectSQL::ConnectSQL() {
	_mysql = mysql_init(NULL);
	mysql_options(_mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(_mysql, MYSQL_INIT_COMMAND, "set names utf8");
}

ConnectSQL::~ConnectSQL() {
	if(!_res)
		mysql_free_result(_res);
}

int ConnectSQL::connectMYSQL() {
	if (!mysql_real_connect(_mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0)) {
		std::cerr << mysql_error(_mysql);
		return 1;
	}
	return 0;
}

int ConnectSQL::queryMYSQL(const char *query) {
	if (mysql_query(_mysql, query)) {
		std::cerr << mysql_error(_mysql) << std::endl;
		mysql_close(_mysql);
		return 1;
	}

	_res = mysql_use_result(_mysql);
	return 0;
}