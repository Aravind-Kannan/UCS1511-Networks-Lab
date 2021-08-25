#include "common.h"
#include <math.h> //pow()

void invert(char message[], int i)
{
    if(message[i] =='1')
    {
        message[i] = '0';
    }
    else if(message[i] =='0')
    {
        message[i] = '1';
    }
}

int calcNumberRedundantBits(int m)
{
    int r = 0;
    for(; pow(2,r) < (m + r + 1); r++);
    return r;
}

void modifyMessage(char message[], int m, int r)
{
    char modifiedMessage[LIMIT];

    int redundantBit = r - 1;
    int msgIndex = 0;
    int i = 0;

    for(; i < m + r; i++)
    {
        // printf("%d %d %d %d\n", i, m + r, (int)pow(2,redundantBit), (i == m - pow(2,redundantBit)));

        if(i == m + r - pow(2,redundantBit))
        {
            modifiedMessage[i] = '0';
            redundantBit--;
        }
        else
        {
            modifiedMessage[i] = message[msgIndex++];
        }
    }

    modifiedMessage[i] = '\0';

    strcpy(message, modifiedMessage);
    // printf("Modified Message: %s\n", modifiedMessage);
}

void calcRedundantBits(char message[], int m, int r)
{
    for(int i = 1; i <= (int)pow(2,r-1); i *= 2)
    {
        int count = 0;

        // printf("\ni: %d\n", i);

        for(int j = i; j <= m + r; j += 2*i)
        {
            int temp = j;
            
            for(int k = 1; k <= i && temp <= m + r; k++)
            {
                if(message[m + r - temp] == '1')
                {
                    count++;
                }
                // printf("%d ", m + r - temp);
                temp++;
            }
        }

        //even parity
        if(count % 2 == 1) // if not, then do
        {
            // printf("(%d [%d])", i, m + r - i);
            message[m + r - i] = '1';
        }
    } 
}
    

int main()
{
    // create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1)
    {
        printf("Error in creating socket...\n"); return -1;
    }

    // server address to bind
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind server_address
    if(bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
    {
        printf("Error in binding address to socket...\n"); return -1;
    }

    // listen on server_socket
    listen(server_socket, 5);

    printf("Waiting for client :/...\n");

    int client_socket = accept(server_socket, NULL, NULL);

    // accept a message and parse it for hamming code by adding redudant bits
    char message[LIMIT];

    printf("Enter message: ");
    scanf(" %[^\n]%*c", message);

    int m = strlen(message);
    int r = calcNumberRedundantBits(m);

    printf("Number of message bits: %d\n", m);
    printf("Number of redundant bits: %d\n", r);

    printf("Message: %s\n", message);
    modifyMessage(message, m, r);
    printf("After Message: %s\n", message);

    calcRedundantBits(message, m, r);

    printf("Parity calc Message: %s\n", message);

    char choice[LIMIT];
    printf("Do you want to introduce error? [Yes/No]: ");
    scanf(" %[^\n]%*c", choice);
    if(strcmp(choice, "Yes") == 0)
    {
        int n = 0;
        printf("1-based index from Left to Right:\n");
        printf("Enter index to introduce error: ");
        do {
            if(n < 0 || n > m + r)
                printf("Re-enter valid index: ");
            scanf("%d", &n);
        } while(n < 0 || n > m + r);
        invert(message, n - 1);
    }

    printf("Error Introduced Message: %s\n", message);

    // write message to client_socket
    write(client_socket, message, sizeof(message));

    // close all OPEN file descriptors
    close(server_socket);

    return 0;
}