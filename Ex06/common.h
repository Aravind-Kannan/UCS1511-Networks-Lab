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
#include <math.h>

#define PORT 7226
#define IP "127.0.0.1"
#define SA struct sockaddr
#define SA_IN struct sockaddr_in
#define LIMIT 1024

void check(int status, char msg[]) {
	if(status == -1) {
		printf("Error in %s...\n", msg);
		exit(0);
	} else {
		printf("Successful in %s...\n", msg);
	}
}

void invert(char msg[], int index) {
	if(msg[index] == '0') {
		msg[index] = '1';
	} else {
		msg[index] = '0';
	}
}
