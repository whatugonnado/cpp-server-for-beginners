#ifndef __CONNECTSQL_H__
#define __CONNECTSQL_H__

#include <mysql.h>
#include <string>

class ConnectSQL {
	SOCKET* _client;
	SOCKET _CLIENT;

	MYSQL *_mysql;
	MYSQL_RES *_res;
	MYSQL_ROW _row;
	std::string _resultString;

public:
	ConnectSQL(void *data);
	~ConnectSQL();

	int connectMYSQL();
	int queryMYSQL(const char *query);
};

#endif // !__CONNECTSQL_H__
#pragma once
