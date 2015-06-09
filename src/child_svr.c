#include "child_svr.h"
#include "bussiness.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>

/*
 *  Constructor
 */
void child_svr_initialization(Child_svr *child_svr,int listenfd)
{   
	child_svr -> listenfd = listenfd;
}

/* 
 * Communicate to client
 */
void child_svr_communication(Child_svr *child_svr)
{
    printf("TCP SERVER IS WAITING!\n");
    child_svr_init_epoll(child_svr);
    child_svr_epoll_add(child_svr,child_svr -> listenfd);
    while (1) {
        int fd_num = epoll_wait(child_svr -> epollfd, child_svr -> events, MAX_EPOLL_SIZE, -1);
        if (fd_num == EOF) {
            perror("child_svr_communication(), epoll_wait()");
            continue;
        }
        while (--fd_num >= 0) {
        	child_svr_io_business(child_svr,fd_num);
        }
    }
}
/*
 * Initialization epoll
 */
int child_svr_init_epoll(Child_svr *child_svr)
{
    int epollfd;
    if((epollfd = epoll_create(MAX_EPOLL_SIZE)) == EOF) {                             
        perror("child_svr_init_epoll(),epoll_create()");
        return EOF;
    }
    child_svr -> epollfd = epollfd;
    return 0;
}

/*
 *  Add one file description symbol for epoll
 */
int child_svr_epoll_add(Child_svr *child_svr,int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    if (epoll_ctl(child_svr -> epollfd, EPOLL_CTL_ADD, fd, &ev)) {
        perror("child_svr_epoll_add(), epoll_ctl()");
        return EOF;
    }
    return 0;
}


/* 
 * Set one file description symbol non block
 */
int child_svr_set_nonblock(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) == EOF) {
        perror("child_svr_set_nonblock(), fcntl()");
        return EOF;
    }
    return 0;
}

/*
 * Respond for io change
 */
void child_svr_io_business(Child_svr *child_svr,int index)
{
    struct epoll_event ev = child_svr -> events[index];
    
    if (ev.data.fd == child_svr -> listenfd) {
        int clt_fd = accept(child_svr -> listenfd, 0, 0);
        if (clt_fd == EOF) {
            perror("child_svr_io_business(), accept()");
            return;
        }
        if(child_svr_set_nonblock(clt_fd) == EOF){
            return;
        }
        if(child_svr_epoll_add(child_svr,clt_fd) == EOF){
            return;
        }
    }else if (ev.events & EPOLLIN){ 
        char *buff = (char *)malloc(BUFF_SIZE);
        memset(buff, 0, BUFF_SIZE);
        int len = child_svr_recv_msg(ev.data.fd, buff);
        if (len == EOF){ 
            perror("child_svr_io_business(), recv_msg()");
            free(buff);
            return;
        }else if (len == 0){
            printf("Connection break!\n");
            epoll_ctl(child_svr -> epollfd,EPOLL_CTL_DEL, ev.data.fd, &ev);
            close(ev.data.fd);
            free(buff);
            return;
        }
        main_service(buff);
        
        Out_data *data = (Out_data*)malloc(sizeof(Out_data));
        data -> fd = ev.data.fd;
        data -> buff = buff;
        ev.data.ptr = data;
        ev.events = EPOLLET | EPOLLOUT;
        if (epoll_ctl(child_svr -> epollfd,EPOLL_CTL_MOD, data -> fd, &ev) == EOF) {
            perror("child_svr_io_business(), modify one event out");
            return;
        }
    }else if (ev.events & EPOLLOUT){
        Out_data *data = (Out_data *)ev.data.ptr;

        if (EOF == child_svr_send_msg(data -> fd,data -> buff)){
            perror("child_svr_io_business(),send_msg()");
        }
        ev.data.fd = data -> fd; 
        ev.events = EPOLLET | EPOLLIN;
        if (epoll_ctl(child_svr -> epollfd,EPOLL_CTL_MOD, ev.data.fd, &ev) == EOF) {
            perror("child_svr_io_business(), modify one event in");
            return;
        }
        free(data -> buff);
        free(data);
    }
    return;
}

/**
 * Receive message,return length
 */
int child_svr_recv_msg(int sockfd, char *buff)
{
    assert(buff);
    FILE *fp; 
    if((fp = fdopen(sockfd,"r+")) == NULL){
        perror("child_svr_recv_msg(),fdopen()");
        return EOF;
    }
    if(fread(buff, BUFF_SIZE, 1, fp) == EOF){
        perror("child_svr_recv_msg(),fread()");
        return EOF;
    }
    printf("Receive message:\n%s\n", buff);
    return strlen(buff);
}

/**
 * Send message,return length
 */

int child_svr_send_msg(int sockfd, char *buff)
{
    assert(buff);
    FILE *fp;
    if((fp = fdopen(sockfd,"r+")) == NULL){
        perror("child_svr_send_msg(),fdopen()");
        return EOF;
    }
    if(fwrite(buff, strlen(buff), 1, fp) == EOF){
        perror("child_svr_send_msg(),fwrite()");
        return EOF;
    }
    fflush(fp);
    printf("Send message:\n%s\n", buff);
    return strlen(buff);
}

