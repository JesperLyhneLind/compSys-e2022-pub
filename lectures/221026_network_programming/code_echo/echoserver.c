/*
 * echoserveri.c - An iterative echo server
 */
/* $begin echoserverimain */
#include "csapp.h"

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    char storage[MAXLINE];
    int is_storage_filled = 0;
    rio_t rio;
    Rio_readinitb(&rio, connfd);

    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
        if (strcmp(buf, "ping\n") == 0) {
            strcpy(buf, "pong\n");
        } else if (strncmp("PUT ", buf, 4) == 0) {
            if (is_storage_filled == 1) {
                strcpy(buf, "VARIABLE FIlLED\n");
            }
            strcpy(storage, &buf[4]);
            strcpy(buf, "OK\n");
            is_storage_filled = 1;
        } else if (strncmp("GET", buf, 3) == 0) {
            if (is_storage_filled == 0) {
                strcpy(buf, "VARIABLE EMPTY\n");
            }
            strcpy(buf, storage);
            is_storage_filled = 0;
        } else {
            strcpy(buf, "BAD REQUEST\n");
        }
        Rio_writen(connfd, buf, strlen(buf));
    }
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        echo(connfd);
        Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */
