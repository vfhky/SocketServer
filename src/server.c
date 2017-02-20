#include "child_svr.h"
#include "server.h"
#include "printnolog.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <assert.h>
#include <fcntl.h>
#include <pthread.h>


/**
 * Initialization
 */
void server_initialization(Server *svr,char *ip, int port)
{
	svr -> ip = ip;
	svr -> port = port;
}

/*
 * Create a TCP Server
 */
void server_create_tcpsvr(Server *svr)
{
    if ((svr -> listenfd = socket(AF_INET, SOCK_STREAM, 0)) == EOF){
        perror("server_create_tcpsvr(),socket()");
        exit(EXIT_FAILURE);
    }

    server_fill_sockaddr(svr);
    
    if (bind(svr -> listenfd, (struct sockaddr *)&(svr -> addr), sizeof(svr -> addr)) == EOF){
        perror("server_create_tcpsvr(),bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(svr -> listenfd,LISTENTQ) == EOF) {
        perror("server_create_tcpsvr(),bind()");
        exit(EXIT_FAILURE);
    }
    
    PRINTNOLOG( "pid=[%d] port=[%d] listenfd=[%d]\n", getpid(), ntohs(svr -> addr.sin_port), svr -> listenfd );
    
    /*
     * Avoid client give up before establish connection
     */
    int flag = fcntl(svr -> listenfd, F_GETFL);
    flag |= O_NONBLOCK;
    if (fcntl(svr -> listenfd, F_SETFL, flag) == EOF) {
        perror("server_create_tcpsvr(), fcntl()");
        exit(EXIT_FAILURE);
    }
}

/**
 * Set TCP server address
 */
void server_fill_sockaddr(Server *svr)
{
	svr -> addr.sin_family = AF_INET;
	svr -> addr.sin_port = htons(svr -> port ? svr -> port : DEF_PORT);
    if (svr -> ip == NULL)
    {
    	svr -> addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
    	svr -> addr.sin_addr.s_addr = inet_addr(svr -> ip);
    }
    
    int i_zero = 1;
    if( setsockopt( svr -> listenfd, SOL_SOCKET, SO_REUSEADDR, &i_zero, sizeof(i_zero) ) )
    {
        close(svr -> listenfd);
        PRINTNOLOG( "setsockopt failed.\n" );
        exit(EXIT_FAILURE);
    }
}

/*
 * Create some process
 */

void server_create_child_svr(Server *svr,int proNum)
{
	/*
	 * Avoid zombie process
	 */
	signal(SIGCHLD, SIG_IGN);
	
	while (proNum--) {
		/**
		 * create process
		 */
		if (fork() == 0)
		{
			Child_svr *child_svr = (Child_svr*)malloc(sizeof(Child_svr));
			child_svr_initialization(child_svr,svr -> listenfd);
			child_svr_communication(child_svr);
		}
	}
    while(1){
        sleep(100);
    }
}
