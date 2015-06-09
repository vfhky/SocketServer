#ifndef CHILD_SVR_H
#define CHILD_SVR_H

#include <sys/epoll.h>


#define MAX_EPOLL_SIZE 10
#define BUFF_SIZE 10240

typedef struct{
    char *buff;
    int fd;
}Out_data;

typedef struct{
    int listenfd;
    int epollfd;
    struct epoll_event events[MAX_EPOLL_SIZE];
}Child_svr;

	void child_svr_initialization(Child_svr *child_svr,int listenfd);
	void child_svr_communication(Child_svr *child_svr);
	void child_svr_io_business(Child_svr *child_svr,int index);
	int child_svr_init_epoll(Child_svr *child_svr);
	int child_svr_epoll_add(Child_svr *child_svr,int clt_fd);
	int child_svr_set_nonblock(int fd);
	int child_svr_recv_msg(int fd, char *buff);
	int child_svr_send_msg(int fd, char *buff);

#endif
