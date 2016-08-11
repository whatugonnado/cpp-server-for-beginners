#include "Server.h"
#include "ConnectSQL.h"
#include "setting.h"

#include <iostream>

std::mutex ioMutex;
std::mutex acceptMutex;

Server::Server() {
	_clientSize = sizeof(_clientInfo);
}

Server::~Server() {
	closesocket(_sock);
	WSACleanup();
}

void Server::Start() {
	if (InitServer()) {
		std::lock_guard<std::mutex> ioLock(ioMutex);
		std::cerr << "Server Start Fail\n";
		return;
	}
	else {
		std::lock_guard<std::mutex> ioLock(ioMutex);
		std::cout << "Server Start!\n";
	}
	Listening();
}

int Server::InitServer() {
	int result;

	result = WSAStartup(MAKEWORD(2, 2), &_wsa);
	if(result){
		std::lock_guard<std::mutex> ioLock(ioMutex);
		std::cerr << "WSA startup fail\n";
		return 1;
	}

	// Create TCP Server Socket
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == INVALID_SOCKET) {
		std::lock_guard<std::mutex> ioLock(ioMutex);
		std::cerr << "Invalid socket\n";
		return 1;
	}

	// edit socket infomation
	_sockInfo.sin_family = AF_INET;
	_sockInfo.sin_port = htons(MY_PORT);
	_sockInfo.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind Socket
	result = bind(_sock, (SOCKADDR *)&_sockInfo, sizeof(_sockInfo));
	if(result) {
		std::lock_guard<std::mutex> ioLock(ioMutex);
		std::cerr << "bind fail " << GetLastError() << "\n";
		return 1;
	}

	// server now listen to clients
	result = listen(_sock, SOMAXCONN);
	if (result) {
		std::lock_guard<std::mutex> ioLock(ioMutex);
		std::cerr << "listen fail " << GetLastError() << "\n";
		return 1;
	}

	return 0;
}

void Server::Listening() {
	while (true) {
		acceptMutex.lock();
		_clientSock = accept(_sock, (SOCKADDR*)&_clientInfo, &_clientSize);
		if (_clientSock == INVALID_SOCKET) {
			std::lock_guard<std::mutex> ioLock(ioMutex);
			std::cerr << "Invalid client socket" << GetLastError() << std::endl;
			acceptMutex.unlock();
			continue;
		}
		else {
			std::thread t1 = runThread();
			t1.detach();
		}
	}
}

void Server::FunctionForClient(void* data) {
	SOCKET* cl = (SOCKET*)data;
	SOCKET CLI = *cl;
	acceptMutex.unlock();

	ConnectSQL SQL;
	SQL.connectMYSQL();
	
	{
		std::lock_guard<std::mutex> ioLock(ioMutex);
		std::cout << "Client Added\n";
	}


	return;
}

std::thread Server::runThread() {
	SOCKET CLIENT = _clientSock;
	return std::thread([=] {FunctionForClient((void*)&CLIENT);});
}