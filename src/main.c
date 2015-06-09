#include "server.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
    Server *svr = (Server*)malloc(sizeof(Server));
    server_initialization(svr,NULL,0);
    server_create_tcpsvr(svr);
    server_create_child_svr(svr,5);
    free(svr);
    exit(EXIT_SUCCESS);
}
