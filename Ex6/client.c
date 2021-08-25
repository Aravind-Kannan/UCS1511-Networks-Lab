#include "common.h"
#include <math.h> //pow()

int calcNumberRedundantBits(int n)
{
    int r = 0;
    for(; pow(2,r) < (n + 1); r++);
    return r;
}

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

int errorDetect(char message[], int m, int r, char check[])
{
    int flag = 0;
    int c = r - 1;
    for(int i = 1; i <= (int)pow(2,r-1); i *= 2)
    {
        int count = 0;
        for(int j = i; j <= m + r; j += 2*i)
        {
            int temp = j;
            
            for(int k = 1; k <= i && temp <= m + r; k++)
            {
                if(message[m + r - temp] == '1')
                {
                    count++;
                }
                temp++;
            }
        }

        //even parity
        if(count % 2 == 1) // if not, then do
        {
            flag = 1;
            check[c--] = '1';
        }
        else
        {
            check[c--] = '0';
        }
    }

    check[r] = '\0';

    return flag;
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

    // check for error in connection
    if(connect(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) 
    {
        printf("Error in connecting to server :/ Try again:(...\n");
        close(server_socket);
        return -1;
    }

    char message[LIMIT];

    read(server_socket, &message, sizeof(message));

    char check[LIMIT];
    int errorBit = 0;

    printf("Received message: %s \n", message);

    int m = strlen(message);
    int r = calcNumberRedundantBits(m);

    m = m - r;

    printf("Number of message bits: %d\n", m);
    printf("Number of redundant bits: %d\n", r);

    printf("Message: %s\n", message);
    
    if(errorDetect(message, m, r, check) == 1)
    {
        printf("Error detected!\n");
        errorBit = (int)strtol(check, NULL, 2);
        printf("Error located at: %s [%d]\n", check, errorBit);
        invert(message, m + r - errorBit);
        printf("Corrected message: %s\n", message);
    }
    else
    {   
        printf("No Error!\n");
    }

    close(server_socket);

    return 0;
}