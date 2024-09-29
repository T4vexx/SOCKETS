#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT "8080"

void check_error(const int code, const char *func_name) {
  if (code == -1) {
    perror(func_name);
    exit(1);
  }
}

int main() {
  struct addrinfo hints, *res;

  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int err = getaddrinfo(NULL, PORT, &hints, &res);

  if(err != 0) {
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(1);
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(sockfd, "socket()");

  err = bind(sockfd, res->ai_addr, res->ai_addrlen);
  check_error(err, "bind()");

  err = listen(sockfd, 5);
  check_error(err, "listen()");

  int clientfd = accept(sockfd, NULL, NULL);
  check_error(clientfd, "accept()");

  char buffer[256];
  while(1) {
    bzero(buffer, sizeof(buffer));

    int bytes = recv(clientfd, buffer, sizeof(buffer), 0);
    printf("Recived %d bytes from client\nMessage: %s\n", bytes, buffer);

    if(strcmp(buffer, "q") == 0 || strcmp(buffer, "Q") == 0) {
      printf("Fechando conexão com o servidor. \n");
      break;
    }

    int j = strlen(buffer) - 1;
    int i;
    char reversed[256];
    for (i=0; i<=256; i++) {
      if(j<0) {
        break;
      }
      reversed[i] = buffer[j];
      j--;
    }
    reversed[i+1] = '\0';
    puts(reversed);
    send(clientfd, reversed, strlen(reversed), 0);
  }

  close(sockfd);
  close(clientfd);
  freeaddrinfo(res);
}