#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv) {
	struct addrinfo hints, *res;

	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int err = getaddrinfo(argv[1], NULL, &hints, &res);
	if(err != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		return 1;
	}

	char ipstr[INET6_ADDRSTRLEN];

	for(struct addrinfo *iter = res; iter != NULL; iter = iter->ai_next) {
		void *addr;
		char *ipver;

		if(iter->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)iter->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver="IPv4";
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)iter->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		inet_ntop(iter->ai_family, addr, ipstr, sizeof(ipstr));

		printf("%s: %s\n", ipver, ipstr);
	}

	freeaddrinfo(res);
	return 0;
}
