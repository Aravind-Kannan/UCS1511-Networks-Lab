#include "common.h"
#define MAX_CLIENTS 3

int main()
{
	SA_IN server, client;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);
	
	int len_server = sizeof(server), len_client = sizeof(client);
	
	int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	check(serverSocket, "creating server socket");
	check(bind(serverSocket, (SA*)&server, sizeof(server)), "binding");
	
	char buffer[LIMIT];
	
	fd_set readfds;
	int clients[3] = {0}, recvfd = 0, port = 0, flag = 0, len_clients = 0;
	struct timeval t;
	t.tv_sec = 30;
	
	do
	{
		FD_ZERO(&readfds);
		FD_SET(serverSocket, &readfds);
		recvfd = select(MAX_CLIENTS + 1, &readfds, NULL, NULL, &t);
		
		for(int i = 0; i < MAX_CLIENTS + 1; i++)
		{
			flag = 1; 
			if(FD_ISSET(i, &readfds))
			{
				bzero(buffer, sizeof(buffer));
				len_client = sizeof(client);
				recvfrom(i, buffer, sizeof(buffer), 0, (SA*)&client, &len_client);
				port = ntohs(client.sin_port);
				
				if(len_clients < MAX_CLIENTS)
				{
					clients[len_clients++] = port;
					flag = len_clients;
				}
				else
				{
					flag = -1;
					for(int k = 0; k < MAX_CLIENTS; k++)
						if(clients[k] == port)
							flag = k + 1;
				}
				
				if(flag == -1)
				{
					printf("Max number of clients(%d) reached...\n", MAX_CLIENTS);
					break;
				}
				
				printf("From Client[%d]: %s\n", port, buffer);
				bzero(buffer, sizeof(buffer));
				printf("To Client[%d]: ", port);
				scanf(" %[^\n]*c", buffer);
				sendto(i, buffer, sizeof(buffer), MSG_CONFIRM, (SA*)&client, len_client);
				
				FD_CLR(serverSocket, &readfds);
			}
			
		}
	
	}while(recvfd != 0);
	
	
	close(serverSocket);
	return 0;
}
