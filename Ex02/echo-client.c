#include "common.h"

int main()
{
	SA_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);
	
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	check(clientSocket, "creating client socket");
	check(connect(clientSocket, (SA*)&server, sizeof(server)), "connecting");
	
	char buffer[LIMIT];
	
	while(1)
	{
		bzero(buffer, sizeof(buffer));
		printf("Enter message to server: ");
		scanf(" %[^;]*c", buffer);
		getchar();
		
		write(clientSocket, buffer, sizeof(buffer));
		
		// terminating condition
		if(strcmp(buffer, "exit") == 0)
			break; 
		
		read(clientSocket, buffer, sizeof(buffer));
		printf("From server---------------------\n%s\n", buffer);
	}
	
	close(clientSocket);
	return 0;
}
