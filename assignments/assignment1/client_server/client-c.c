/*****************************************************************************
 * client-c.c                                                                 
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

#define SEND_BUFFER_SIZE 2048



int send_all(int s, char *buf, size_t size) {
  size_t total_sended = 0;
  while(total_sended < size) {
    size_t sended = send(s, buf+total_sended, size-total_sended, MSG_NOSIGNAL);
    if(sended == -1){
      perror("send");
      return -1;
    }  
    total_sended += sended;
  }
  return 0;
}

void filladdrinfo(char *server_ip, char *server_port, struct addrinfo **serverinfo) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo(server_ip, server_port, &hints, serverinfo);
}
/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/
int client(char *server_ip, char *server_port) {
  char buf[SEND_BUFFER_SIZE];
  struct addrinfo *serverinfo = 0;
  filladdrinfo(server_ip, server_port, &serverinfo);
  if(serverinfo) {
    int s = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
    if(s == -1) perror("socket");
    if(connect(s, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1) perror("connect");
    // read from stdin to buf and then send it
    memset(buf, 0, sizeof buf);
    size_t readed = 0;
    while((readed = read(0, buf, sizeof buf)) > 0) {
    // while(fgets(buf, sizeof buf, stdin)) {
    //   readed = strlen(buf);
      if(send_all(s, buf, readed) != 0) perror("sendall");
      memset(buf, 0, sizeof buf);
    }
    close(s);
  }
  return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv) {
  char *server_ip;
  char *server_port;

  if (argc != 3) {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
    exit(EXIT_FAILURE);
  }

  server_ip = argv[1];
  server_port = argv[2];
  return client(server_ip, server_port);
}
