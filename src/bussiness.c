#include "bussiness.h"

int main_service(char *buff)
{
	char *head = "HTTP/1.1 200 0K \r\nServer: my server\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s\r\n";
	char *html = "<html><h1>It works!</h1></html>";
	int length = (int)strlen(html);
    sprintf(buff,head,length,html);
    return 0;
}

