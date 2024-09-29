#define _GNU_SOURCE

#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT "8080"

void check_error(const int code, const char *func_name)
{
  if (code == -1)
  {
    perror("func_name");

    exit(1);
  }
}

int main()
{
  struct addrinfo hints, *res;
  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  int err = getaddrinfo(NULL, PORT, &hints, &res);
  if (err != 0)
  {
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
  check_error(err, "accept()");

  int buffer;
  int bytes = recv(clientfd, &buffer, sizeof(buffer), 0);

  printf("\n[SERVIDOR] Resposta da soma é %d\n", buffer);

  close(sockfd);
  close(clientfd);
  freeaddrinfo(res);
}