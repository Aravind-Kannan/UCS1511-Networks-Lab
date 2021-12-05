#include "common.h"

int main() {
	SA_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);
	
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	check(clientSocket, "creating client socket");
	check(connect(clientSocket, (SA*)&server, sizeof(server)), "connecting");
	
	ARP sent, recvd;
	char localIP[LIMIT], localMAC[LIMIT], buffer[LIMIT];
	printf("Enter Local IP:"); getIP(localIP);
	printf("Enter Local MAC:"); getMAC(localMAC);
	
	while(1) {
		read(clientSocket, (ARP*)&recvd, sizeof(recvd));
		
		bzero(&sent, sizeof(sent));
		
		if(recvd.flag == KILL) {
			printf("Exiting...\n");
			break;
		} else if(recvd.flag == DATA) {
			printARP(recvd);
			printf("Data: %s\n", recvd.data);
		} else if(recvd.flag == NORM) {
			printARP(recvd);
			if(strcmp(recvd.destIP, localIP) == 0) {
				printf("MAC match...\n");
				sent.mode = 2;
				strcpy(sent.srcIP, recvd.destIP);
				strcpy(sent.srcMAC, localMAC);
				strcpy(sent.destIP, recvd.srcIP);
				strcpy(sent.destMAC, recvd.srcMAC);
				sent.flag = MATCH;
				printARP(sent);
			} else {
				printf("MAC didn't match...\n");
				sent.flag = NO_MATCH;
			}
		}
		
		write(clientSocket, (ARP*)&sent, sizeof(sent));
	}
	close(clientSocket);

	return 0;
}
