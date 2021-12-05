// Start: 10:55 AM
#include "common.h"

int calcRedBits(int m) {
	int r = 0;
	for(; pow(2,r) < m + r; r++);
	return r;
}

void findRedBits(char msg[], int m, int r) {
	char modifiedMsg[LIMIT];
	int redBits = r - 1;
	int index = 0;
	int i = 0;
	for(; i <= m + r; i++) {
		if (i == m + r - pow(2, redBits)) {
			modifiedMsg[i] = '0';
			redBits--;
		} else {
			modifiedMsg[i] = msg[index++];
		}
	}
	modifiedMsg[i] = '\0';
	strcpy(msg, modifiedMsg);
}

void valueRedBits(char msg[], int m, int r) {
	for(int i = 1; i <= pow(2, r - 1); i *= 2) {
		int count = 0;
		for(int j = i; j <= m+r; j += 2*i) {
			int temp = j;
			for(int k = 1; k <= i && temp <= m + r; k++, temp++) {
				if(msg[m + r - temp] == '1')
					count++;
			}
		}
		
		if(count%2 == 1) {
			msg[m + r - i] = '1';
		}
	}
}

int main() {
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
	
	char message[LIMIT];
	printf("Enter message = ");
	scanf(" %[^\n]*c", message);
	
	int m = strlen(message);
	int r = calcRedBits(m);
	
	findRedBits(message, m, r);
	printf("Before Parity intro message: %s\n", message);
	
	valueRedBits(message, m, r);
	printf("Parity introduced message  : %s\n", message);
	
	// introduce error
	int error;
	do {
		printf("Enter 1-Based index from right: ");
		scanf("%d",&error);
	} while(error < 1 || error > m + r);
	invert(message, m + r - error);
	
	printf("Error introduced message   : %s\n", message);
	
	write(clientSocket, message, sizeof(message));
	
	close(serverSocket);	
	
	return 0;
}
