#include "common.h"

// Testcase: www.africau.edu/images/default/sample.pdf

int main(int argc, char* argv[]) {

	if (argc != 2)
	{
		printf("Usage: ./client [URL]\n");
		return -1;
	}
	
	char params[2][LIMIT];
	
	if(strstr(argv[1], "/"))
	{
		int i = 0, j = 0;
		for(; argv[1][i] != '/'; i++)
		{
			params[0][i] = argv[1][i];
		}
		params[0][i + 1] = '\0';
		
		for(; argv[1][i] != '\0'; i++, j++)
		{
			params[1][j] = argv[1][i];
		}
		params[0][j + 1] = '\0';
	}
	else
	{
		strcpy(params[0], argv[1]);
		strcpy(params[1], "/");
	}
	
	char ip[LIMIT], request[LIMIT * LIMIT], response[LIMIT];
	
	struct hostent* hent = gethostbyname(params[0]);
	strcpy(ip, inet_ntoa(*((struct in_addr*)hent->h_addr_list[0])));
	printf("\nIP address of %s: %s\n", params[0], ip);
	printf("params[0] = %s\nparams[1] = %s\n", params[0], params[1]);

	// Socket and paste to file
	SA_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(ip);
	
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	int connStatus = connect(clientSocket, (SA*)&server, sizeof(server));
	sprintf(request, "GET %s HTTP/1.1\r\nHost: %s \r\n\r\n", params[1], params[0]);
	
	send(clientSocket, request, sizeof(request), 0);
	
	int bytes = 0;
	
	char fileName[LIMIT];
	printf("Enter file name: ");
	scanf("%[^\n]*c", fileName);
	
	int fd = creat(fileName, S_IRWXU);
	
	while((bytes = recv(clientSocket, response, sizeof(response), 0)) != 0)
	{
		write(fd, response, bytes);
	}
	
	close(fd);
	close(clientSocket);
	
	return 0;
}
