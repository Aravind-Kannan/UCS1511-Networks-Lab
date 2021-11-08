#include "common.h"

// testcase: ./client www.africau.edu/images/default/sample.pdf
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage: ./client [URL]\n");
		return -1;
	}
	
	char params[2][LIMIT];
	
	// split into params for GET request parsing
	if(strstr(argv[1], "/"))
	{
		int i = 0, j = 0;
		
		for(; argv[1][i] != '/'; i++)
			params[1][i] = argv[1][i];
		params[1][i + 1] = '\0';
		
		for(; argv[1][i] != '\0'; i++, j++)
			params[0][j] = argv[1][i];
		params[0][j + 1] = '\0';
	}
	else
	{
		strcpy(params[0], "/");
		strcpy(params[1], argv[1]);
	}
	
	// find ip
	char ip[LIMIT];
	struct hostent* hent = gethostbyname(params[1]);
	strcpy(ip, inet_ntoa(*((struct in_addr*)hent->h_addr_list[0])));
	printf("hostname: %s\nresource: %s\nip: %s\n", params[1], params[0], ip);
	
	// traditional client set-up
	SA_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(ip);
	
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	check(clientSocket, "creating client socket");
	check(connect(clientSocket, (SA*)&server, sizeof(server)), "connecting");
	
	char request[LIMIT * LIMIT];
	sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", params[0], params[1]);
	
	char filename[LIMIT], buffer[LIMIT];
	printf("Enter a file to download to: ");
	scanf(" %[^\n]*c", filename);
	
	int fd = open(filename, O_CREAT | O_WRONLY, S_IRWXU), bytes = 0;
	write(clientSocket, request, sizeof(request));
	
	while(bytes = read(clientSocket, buffer, sizeof(buffer)))
	{
		// write(0, buffer, bytes);
		write(fd, buffer, bytes);
	}
	
	close(fd);
	close(clientSocket);
	return 0;
}
