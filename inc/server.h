#ifndef SERVER_H
#define SERVER_H

#define LISTENTQ 5
#define DEF_PORT 8080

#include <netinet/in.h>

typedef struct {
	char *ip;
	unsigned int port;
	int listenfd;
	struct sockaddr_in addr;
} Server;

	void server_initialization(Server *svr,char *ip, int port);
	void server_create_tcpsvr(Server *svr);
	void server_fill_sockaddr(Server *svr);
	void server_create_child_svr(Server *svr,int proNum);

#endif
