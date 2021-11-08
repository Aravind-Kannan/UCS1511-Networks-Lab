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
	
	char filename[LIMIT], buffer[LIMIT];
	
	printf("Enter filename: ");
	scanf(" %[^\n]*c", filename);
	
	write(clientSocket, filename, sizeof(filename));
	int bytes = read(clientSocket, buffer, sizeof(buffer));
	
	if(bytes == 0)
	{
		printf("File doesn't exist :(\n");
	}
	else
	{
		printf("Enter filename to store in: ");
		scanf(" %[^\n]*c", filename);
		
		int fd = open(filename, O_CREAT | O_WRONLY, S_IRWXU);
		
		do
		{
			write(fd, buffer, bytes);
		}
		while(bytes = read(clientSocket, buffer, sizeof(buffer)));
		
		close(fd);
	}
	
	
	close(clientSocket);
	return 0;
}
