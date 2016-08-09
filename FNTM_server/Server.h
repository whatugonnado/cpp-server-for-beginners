#ifndef __SERVER_H__
#define __SERVER_H__

#include <WinSock2.h>
#include <thread>
#include <mutex>
#include <string>

class Server {
	SOCKET _sock;
	SOCKET _clientSock;

	WSADATA _wsa;
	sockaddr_in _sockInfo, _clientInfo;
	int _clientSize;
public:
	Server();
	~Server();
	
	void Start();
	int InitServer();
	void Listening();
	void FunctionForClient(void* data);

	std::thread runThread();
};

#endif // !__SERVER_H__
#pragma once
