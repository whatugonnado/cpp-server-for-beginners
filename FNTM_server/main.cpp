#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Server.h"

#pragma comment (lib, "Ws2_32.lib")

int main() {

	Server server;
	server.Start();

	return 0;
}

