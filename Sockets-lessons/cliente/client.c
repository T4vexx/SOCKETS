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

  int err = getaddrinfo("127.0.0.1", PORT, &hints, &res);

  if(err != 0) {
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(1);
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(sockfd, "socket()");

  err = connect(sockfd, res->ai_addr, res->ai_addrlen);
  check_error(err, "connect()");

  char buffer[256];
  while(1) {
    bzero(buffer, sizeof(buffer));

    printf("Message to send ('q' or 'Q' to finish): ");
    fgets(buffer, sizeof(buffer), stdin);
    if(strlen(buffer) - 1 <= 0) {
      continue;
    } 

    buffer[strlen(buffer) - 1] = '\0';
    int bytes = send(sockfd, buffer, strlen(buffer), 0);

    if(strcmp(buffer, "q") == 0 || strcmp(buffer, "Q") == 0) {
      printf("Fechar conexão com o cliente.\n");
      break;
    }

    bytes = recv(sockfd, buffer, sizeof(buffer), 0);

    printf("Message reversed: %s\n", buffer);
    putchar('\n');
  }

  close(sockfd);
  freeaddrinfo(res);
}