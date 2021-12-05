#include "common.h"

int calcRedBits(int n) {
	int r = 0;
	for(; pow(2,r) < n; r++);
	return r;
}

void extractMsg(char modifiedMsg[], int m, int r) {
	char msg[LIMIT];
	int redBits = r - 1;
	int index = 0;
	int i = 0;
	for(; i <= m + r; i++) {
		if (i == m + r - pow(2, redBits)) {
			redBits--;
		} else {
			msg[index++] = modifiedMsg[i];
		}
	}
	msg[i] = '\0';
	strcpy(modifiedMsg, msg);
}

int calcCheckBits(char msg[], int m, int r, char check[]) {
	
	int c = r - 1;
	int flag = 0;

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
			printf("check[%d] = 1\n", c);
			check[c--] = '1';
			flag = 1;
		} else {
			printf("check[%d] = 0\n", c);
			check[c--] = '0';
		}
	}
	
	check[r] = '\0';
	printf("check[%d] = EOS\n", r);
	
	return flag;
}

int main() {
	SA_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);
	
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	check(clientSocket, "creating server socket");
	check(connect(clientSocket, (SA*)&server, sizeof(server)), "connecting");
	
	char message[LIMIT];
	read(clientSocket, message, sizeof(message));
	printf("Received Msg : %s\n", message);
	
	int n = strlen(message);
	int r = calcRedBits(n);
	int m = n - r;
	
	char check[LIMIT];
	int flag = calcCheckBits(message, m, r, check);
	if(flag == 1) {
		int error = (int)strtol(check, NULL, 2);
		invert(message, m + r - error);
		extractMsg(message, m, r);
		printf("Error at %s (%d)\n", check, error);
		printf("Corrected Msg: %s\n", message);
	} else {
		printf("No error detected!\n");
		extractMsg(message, m, r);
		printf("Message	     : %s\n", message);
	}
	
	
	
	close(clientSocket);	
	
	return 0;
}
