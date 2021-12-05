#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>

#define IP "127.0.0.1"
#define PORT 7226
#define LIMIT 1024
#define SA struct sockaddr
#define SA_IN struct sockaddr_in
#define TABLE struct entry

struct entry{
	char name[LIMIT];
	char ip[LIMIT][LIMIT];
	int len;
};


void check(int status, char msg[]) {
	if(status == -1) {
		printf("Error in %s...\n", msg);
		exit(0);
	} else {
		printf("Successful in %s...\n", msg);
	}
}
