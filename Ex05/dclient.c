#include "common.h"

int main() {
	SA_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);
	
	int len_server = sizeof(server);
	
	int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	check(clientSocket, "creating client socket");
	check(connect(clientSocket, (SA*)&server, sizeof(server)), "connecting");
	
	char buffer[LIMIT];
	
	while(1) {
		printf("Enter domain: ");
		scanf(" %[^\n]*c", buffer);
		
		if(strcmp(buffer, "exit") == 0)
			break;
		
		sendto(clientSocket, buffer, sizeof(buffer), MSG_CONFIRM, (SA*)&server, len_server);
		recvfrom(clientSocket, buffer, sizeof(buffer), MSG_WAITALL, (SA*)&server, &len_server);
		printf("From DNS server - IP address(es):\n %s\n", buffer);
	}
	
	close(clientSocket);

	return 0;
}
