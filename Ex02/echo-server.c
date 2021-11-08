#include "common.h"

int main()
{
	SA_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);
	
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	check(serverSocket, "creating server socket");
	
	check(bind(serverSocket, (SA*)&server, sizeof(server)), "binding");
	check(listen(serverSocket, 5), "listening");
	
	int clientSocket = accept(serverSocket, NULL, NULL);
	check(clientSocket, "accepting");
	
	char buffer[LIMIT];
	
	while(1)
	{
		bzero(buffer, sizeof(buffer));
		read(clientSocket, buffer, sizeof(buffer));
		
		// terminating condition
		if(strcmp(buffer, "exit") == 0)
			break; 
		
		printf("From server---------------------\n%s\n", buffer);
		write(clientSocket, buffer, sizeof(buffer));
	}
	
	close(clientSocket);
	close(serverSocket);
	return 0;
}
