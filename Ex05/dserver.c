#include "common.h"
#define MAX_CLIENTS 3
#define ROWS 6

// Print table
void printTable(TABLE table[], int len_table) {
	printf("Domain\tIP\n");
	for(int i = 0; i < len_table; i++) {
		printf("%s\t", table[i].name);
		for(int j = 0; j < table[i].len; j++) {
			printf("%s,", table[i].ip[j]);
		}
		printf("\n");
	}
}

// Resolve domain
int resolve(char buffer[], TABLE table[], int len_table) {
	int flag = -1;
	for(int i = 0; i < len_table; i++) {
		printf("%s %s %d\n", buffer, table[i].name, strcmp(buffer, table[i].name));
		if(strcmp(buffer, table[i].name) == 0) {
			flag = 0;
			memset(buffer, 0, strlen(buffer));
			for(int j = 0; j < table[i].len; j++) {
				strcat(buffer,table[i].ip[j]);
				strcat(buffer, "\n");
			}
		}
	}
	return flag;
}

int main() {
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
	int clients[MAX_CLIENTS] = {0}, len_clients = 0, recvfd = 0, port = 0, flag = 0; 
	
	struct timeval t;
	t.tv_sec = 30;
	
	// Define table 
	TABLE table[ROWS];
	int len_table = 0;
	printf("Enter number of domains: ");
	scanf(" %d", &len_table);
	
	for(int i = 0; i < len_table; i++) {
		printf("Domain %d-----------\nEnter name:", i);
		scanf(" %[^\n]*c", table[i].name);
		printf("Enter number of ip address(es): ");
		scanf(" %d", &table[i].len);
		for(int j = 0; j < table[i].len; j++) {
			printf("Enter ip addr %d:", j);
			scanf(" %[^\n]*c", table[i].ip[j]);
		}
	}
	
	printTable(table, len_table);
	
	do {
		FD_ZERO(&readfds);
		FD_SET(serverSocket, &readfds);
		recvfd = select(MAX_CLIENTS + 1, &readfds, NULL, NULL, &t);
		
		for(int i = 0; i < MAX_CLIENTS + 1; i++) {
			flag = 1;
			
			if(FD_ISSET(i, &readfds)) {
				bzero(buffer, sizeof(buffer));
				recvfrom(i, buffer, sizeof(buffer), 0, (SA*)&client, &len_client);
				port = client.sin_port;
				
				if(len_clients < MAX_CLIENTS) {
					clients[len_clients++] = port;
					flag = len_clients;
				} else {
					flag = -1;
					for(int k = 0; k < MAX_CLIENTS; k++)
						if(clients[k] == port)
							flag = k + 1;
				}	
				
				if(flag == -1) {
					printf("Maximum number of clients (%d) reached...\n", MAX_CLIENTS);
					break;
				}			
				
				printf("From client[%d]: %s\n", port, buffer);
				
				if(resolve(buffer, table, len_table) == -1)
					strcpy(buffer, "Not found!\n");
				
				sendto(i, buffer, sizeof(buffer), MSG_CONFIRM, (SA*)&client, len_client);
				FD_CLR(serverSocket, &readfds);
				
				// Modify table
				char choice[LIMIT], domain[LIMIT];
				int flag = -1;
				printf("Do you want to modify? :");
				scanf(" %[^\n]*c", choice);
				
				if(strcmp(choice, "Yes") == 0) {
					printf("Enter domain to add ip:");
					scanf(" %[^\n]*c", domain);
					
					for(int k = 0; k < len_table; k++) {
						if(strcmp(buffer, table[k].name) == 0) {
							flag = 0;
							printf("Enter new ip to add:");
							scanf(" %[^\n]*c", table[k].ip[table[k].len++]);
						}
					}
					
					if(flag == -1) {
						printf("Domain not found!\n");
					}
									
				}
				
			}
		}
	} while(recvfd != 0);
	
	close(serverSocket);

	return 0;
}
