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

// Se quiser aumentar o tanto de workers, deve ser adicionado aqui as novas portas, e na função set_worker_number
// também deve ser modivicado o switch case, de resto tudo já funciona de forma dinâmica e genérica
#define PORT_MANAGER "8080"
#define PORT_WORKER0 "8081"
#define PORT_WORKER1 "8082"
#define PORT_WORKER2 "8083"
#define PORT_WORKER3 "8084"
#define PORT_WORKER4 "8085"
#define PORT_WORKER5 "8086"
#define PORT_WORKER6 "8087"
#define PORT_WORKER7 "8088"
// Na lógica atual os workers começam no id 0 e terminal no id 7,
// porém é possível aumentar essa quantidade de acordo com uma árvore binária
// Exemplo: se quiser com 16 Workers, teremos Worker0 até o Worker15
#define NUMERO_WORKES 8
#define MAX_LEVEL (log2(NUMERO_WORKES) - 1)

typedef struct worker
{
  int id;
  char port[6];
  int arg;
  int level;
} Worker;

void check_error(const int code, const char *func_name)
{
  if (code == -1)
  {
    perror(func_name);
    exit(1);
  }
}

char *set_worker_number(int id)
{
  switch (id)
  {
  case 0:
    return PORT_WORKER0;
  case 1:
    return PORT_WORKER1;
  case 2:
    return PORT_WORKER2;
  case 3:
    return PORT_WORKER3;
  case 4:
    return PORT_WORKER4;
  case 5:
    return PORT_WORKER5;
  case 6:
    return PORT_WORKER6;
  case 7:
    return PORT_WORKER7;
  default:
    return NULL;
  }
}

struct addrinfo *send_data(int *sockfd, Worker *worker, struct addrinfo *hints)
{
  struct addrinfo *res;
  int err = getaddrinfo("127.0.0.1", set_worker_number(worker->id - pow(2, worker->level)), hints, &res);
  if (err != 0)
  {
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(1);
  }

  *sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(*sockfd, "socket()");

  do
  {
    err = connect(*sockfd, res->ai_addr, res->ai_addrlen);
    sleep(2);
  } while (err == -1);

  int bytes = send(*sockfd, worker, sizeof(*worker), 0);
  check_error(bytes, "send()");

  char response[256];
  bytes = recv(*sockfd, response, sizeof(response), 0);
  check_error(bytes, "recv()");
  if (strcmp(response, "WAIT") == 0)
  {
    sleep(1);
    return send_data(sockfd, worker, hints);
  }
  else
  {
    worker->level = -1;
    close(*sockfd);
    return res;
  }
}

void send_response_to_manager(int soma)
{
  struct addrinfo hints, *res;
  int sockfd;

  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int err = getaddrinfo("127.0.0.1", "8080", &hints, &res);
  if (err != 0)
  {
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(1);
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(sockfd, "socket()");

  err = connect(sockfd, res->ai_addr, res->ai_addrlen);
  check_error(err, "connect()");
  printf("soma: %d", soma);
  int bytes = send(sockfd, &soma, sizeof(soma), 0);
  check_error(bytes, "send()");
}

struct addrinfo *recive_data(int *sockfd, Worker *worker, struct addrinfo *hints)
{
  Worker response;
  struct addrinfo *res;

  int err = getaddrinfo("127.0.0.1", worker->port, hints, &res);
  if (err != 0)
  {
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(1);
  }

  *sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(*sockfd, "socket()");

  int opt = 1;
  if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
  {
    perror("failed");
    exit(EXIT_FAILURE);
  }
  err = bind(*sockfd, res->ai_addr, res->ai_addrlen);
  check_error(err, "bind()");

  err = listen(*sockfd, 8);
  check_error(err, "listen()");

  int clientfd = accept(*sockfd, NULL, NULL);
  check_error(clientfd, "accept()");

  int bytes = recv(clientfd, &response, sizeof(response), 0);
  if (response.id != worker->id + pow(2, worker->level))
  {
    const char *message = "WAIT";
    bytes = send(clientfd, message, strlen(message), 0);
    check_error(bytes, "send()");
  }
  else
  {
    const char *message = "OK";
    bytes = send(clientfd, message, strlen(message), 0);
    check_error(bytes, "send()");
  }

  worker->arg = worker->arg + response.arg;
  if (worker->level == MAX_LEVEL)
  {
    printf("%d ultimo", worker->arg);
    send_response_to_manager(worker->arg);
    return res;
  }
  worker->level = worker->level + 1;

  close(*sockfd);
  return res;
}

void borboleta(Worker *worker)
{
  struct addrinfo hints, *res;
  int sockfd;
  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // Calcula o id do worker correspondente no nível atual da árvore
  int paired_worker_id = worker->id ^ (int)pow(2, worker->level);

  // Se o id do worker for maior que o do par, ele envia os dados; caso contrário, recebe
  if (worker->id > paired_worker_id)
  {
    res = send_data(&sockfd, worker, &hints);
  }
  else
  {
    res = recive_data(&sockfd, worker, &hints);
    borboleta(worker);
  }

  if (res != NULL)
  {
    freeaddrinfo(res);
  }
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    fprintf(stderr, "Uso: %s <id_worker> <arg>\n", argv[0]);
    exit(1);
  }
  Worker worker;
  worker.id = atoi(argv[1]);
  worker.arg = atoi(argv[2]);
  worker.level = 0;
  strcpy(worker.port, set_worker_number(worker.id));

  borboleta(&worker);

  return 0;
}
