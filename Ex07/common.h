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

#define PORT 80
#define LIMIT 1024
#define SA_IN struct sockaddr_in
#define SA struct sockaddr

void check(int status, char msg[])
{
	if(status == -1)
	{
		printf("Error in %s...\n", msg);
		exit(0);
	}
	else
	{
		printf("Successful in %s...\n", msg);
	}
}
