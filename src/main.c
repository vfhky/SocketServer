#include "main.h"
#include "server.h"
#include "printnolog.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    Server *svr = (Server*) malloc(sizeof(Server));
    server_initialization(svr, NULL, PORT);
    server_create_tcpsvr(svr);

    server_create_child_svr(svr, CHILD_SVR_NUM);
    free(svr);

    exit(EXIT_SUCCESS);
}
