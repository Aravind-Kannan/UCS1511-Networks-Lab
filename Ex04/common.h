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
#define SA struct sockaddr
#define SA_IN struct sockaddr_in
#define ARP struct arp
#define LIMIT 1024

#define NO_MATCH -2
#define MATCH 2 
#define KILL -1
#define NORM 0
#define DATA 1

struct arp{
	int mode, flag;
	char srcIP[LIMIT], destIP[LIMIT];
	char srcMAC[LIMIT], destMAC[LIMIT];
	char data[LIMIT];
};

int checkIP(char ip[]) {
	char substr[LIMIT];
	int counter = 0, index = 0;
	for(int i = 0; i <= strlen(ip); i++) {
		if(ip[i] == '.' || ip[i] == '\0') {
			substr[index] = '\0';
			counter++;
			int number = atoi(substr);
			if(number < 0 || number > 255){
				printf("Format: [0-255].[0-255].[0-255].[0.255]\nPlease re-enter");
				return 0;
			}
			index = 0;
		} else {
			substr[index++] = ip[i];
		}
	}
	
	if(counter != 4){
		printf("Format: [0-255].[0-255].[0-255].[0.255]\nPlease re-enter");
		return 0;
	}
}

void getIP(char ip[]) {
	do {
		scanf(" %[^\n]*c", ip);
	}while(checkIP(ip) == 0);
}

int checkMAC(char mac[]) {
	char substr[LIMIT];
	int counter = 0, index = 0;
	for(int i = 0; i <= strlen(mac); i++) {
		if(mac[i] == '-' || mac[i] == '\0') {
			substr[index] = '\0';
			counter++;
			int number = strtol(substr, NULL, 16);
			if(number < 0 || number > 255){
				printf("Format: [00-FF]-[00-FF]-[00-FF]-[00-FF]-[00-FF]-[00-FF]\nPlease re-enter");
				return 0;
			}
			index = 0;
		} else {
			substr[index++] = mac[i];
		}
	}
	
	if(counter != 6){
		printf("Format: [00-FF]-[00-FF]-[00-FF]-[00-FF]-[00-FF]-[00-FF]\nPlease re-enter");
		return 0;
	}
}

void getMAC(char mac[]) {
	do {
		scanf(" %[^\n]*c", mac);
	}while(checkMAC(mac) == 0);
}

void check(int status, char msg[]) {
	if(status == -1) {
		printf("Error in %s...\n", msg);
		exit(0);
	} else {
		printf("Successful in %s...\n", msg);
	}
}

void printARP(ARP packet) {
	printf("%d|%s|%s|%s|%s\n", packet.mode, packet.srcMAC, packet.srcIP, packet.destMAC, packet.destIP);
}
