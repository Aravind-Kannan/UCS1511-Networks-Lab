#include "common.h"
#define MAX_CLIENTS 2

int main() {
	SA_IN server, client;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);
	
	int len_client = sizeof(client);
	
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	check(serverSocket, "creating client socket");
	check(bind(serverSocket, (SA*)&server, sizeof(server)), "binding");
	check(listen(serverSocket, MAX_CLIENTS), "listenting");
	
	int clientSockets[MAX_CLIENTS];
	for(int i = 0; i < MAX_CLIENTS; i++) {
		clientSockets[i] = accept(serverSocket, (SA*)&client, &len_client);
		if (clientSockets[i] != -1)
			printf("Client[%d] accepted...\n", client.sin_port);
	}
	
	ARP sent, recvd;
	sent.mode = 1;
	sent.flag = NORM;
	printf("Enter srcIP:"); getIP(sent.srcIP);
	printf("Enter srcMAC:"); getMAC(sent.srcMAC);
	printf("Enter destIP:"); getIP(sent.destIP);
	strcpy(sent.destMAC, "00-00-00-00-00-00");
	printf("Enter data:"); scanf(" %[^\n]*c", sent.data);
	
	int identified_fd = -1;
	
	while(1) {
	
		for(int i = 0; i < MAX_CLIENTS; i++) {
			write(clientSockets[i], (ARP*)&sent, sizeof(sent));
			read(clientSockets[i], (ARP*)&recvd, sizeof(recvd));
				
			if(recvd.flag == MATCH) {
				printf("Match found...\n");
				identified_fd = clientSockets[i];
				strcpy(sent.destMAC, recvd.srcMAC);
			}
		}
		
		if(identified_fd == -1) {
			printf("No match...\n");
			break;
		}
		
		sent.flag = DATA;
		write(identified_fd, (ARP*)&sent, sizeof(sent));
		read(identified_fd, (ARP*)&recvd, sizeof(recvd));
		printf("Data sent...\n");
		
		bzero(&sent, sizeof(sent));
		sent.flag = KILL;
		for(int i = 0; i < MAX_CLIENTS; i++) {
			printf("Killing client %d...\n", i);
			write(clientSockets[i], (ARP*)&sent, sizeof(sent));
		}
	
	}
	
	close(serverSocket);

	return 0;
}
