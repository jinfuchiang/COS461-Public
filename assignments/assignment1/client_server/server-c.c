/*****************************************************************************
 * server-c.c                                                                 
 * Name:
 * NetId:
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048


int get_server_socket(const char *server_port) {
  struct addrinfo hints, *serverinfo;
  int s;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if(getaddrinfo(NULL, server_port, &hints, &serverinfo) == -1) perror("getaddrinfo");
  if(serverinfo)
    s = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
  else
    return -1;
  if(s == -1) perror("socket");
  if(bind(s, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1) {
    perror("bind");
    exit(1);
  }
  if(listen(s, QUEUE_LENGTH) == -1) {
    perror("listen");
  }
  
  return s;
}
/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
*/
int server(const char *server_port) {
  char buf[RECV_BUFFER_SIZE];
  int listener = get_server_socket(server_port);
  int doer;
  struct sockaddr_storage clientaddr;
  socklen_t addr_size;
  while((doer = accept(listener, (struct sockaddr*)&clientaddr, &addr_size)) != -1) {
    memset(buf, 0, sizeof(buf));
    size_t received = 0;
    while((received = recv(doer, buf, RECV_BUFFER_SIZE, 0)) > 0) {
      write(1, buf, received);
      memset(buf, 0, sizeof(buf));
    }
    if(received) perror("recv");
    close(doer);
  }
  return 0;
}
/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv) {
  char *server_port;

  if (argc != 2) {
    fprintf(stderr, "Usage: ./server-c [server port]\n");
    exit(EXIT_FAILURE);
  }

  server_port = argv[1];
  return server(server_port);
}
