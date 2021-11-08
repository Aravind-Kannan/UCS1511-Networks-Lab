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
	
	char filename[LIMIT], buffer[LIMIT];
	
	int bytes = read(clientSocket, filename, sizeof(filename));
	int fd = open(filename, O_RDONLY);
	
	if(fd == -1)
	{
		write(clientSocket, NULL, 0);
		printf("File doesn't exist :(\n");
	}
	else
	{
		while(bytes = read(fd, buffer, sizeof(buffer)))
		{
			write(clientSocket, buffer, bytes);
		}
		
		close(fd);
	}
	
	close(clientSocket);
	close(serverSocket);
	return 0;
}
