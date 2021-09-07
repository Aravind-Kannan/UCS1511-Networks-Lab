#include <stdio.h>
#include <stdlib.h>
#include <string.h>	 // strcpy()
#include <strings.h> // bzero()

#include <sys/types.h>  // socket()
#include <sys/socket.h> // socket()

#include <netinet/in.h>
#include <arpa/inet.h> 

#include <unistd.h>

#include <netdb.h> // gethostbyname()

#include <sys/types.h> // open()
#include <sys/stat.h>  // open()
#include <fcntl.h>     // open()


#define LIMIT 1024
#define PORT 80

int main(int argc, char* argv[]) {

    // Argument validation
    if (argc != 2) {
        printf("Error!\nUsage: ./download [webpage-link]\n");
        return -1;
    }

    // Process Arguments
    char arr[2][LIMIT];
    if ((strstr(argv[1], "/"), "/") == 0)
    {

        strcpy(arr[0], strtok(argv[1], "/"));
        sprintf(arr[0], "/%s", arr[1]);
        strcpy(arr[1], strtok(NULL, " "));
    }
    else
    {
        strcpy(arr[0], "/");
        strcpy(arr[1], argv[1]);
    }

    // Find IPv4
    char target[LIMIT];
    strcpy(target, argv[1]);
    printf("Webpage: %s\n", target);

    struct hostent* hent = gethostbyname(target);

    // Convert: char* to in_addr* (IPv4 conversion)
    // Dereference and supply to: char *inet_ntoa(struct in_addr in);
    // printf("%s \n", inet_ntoa(*((struct in_addr *)hent->h_addr_list[0])));

    char ip[LIMIT];
    strcpy(ip, inet_ntoa(*((struct in_addr*)hent->h_addr_list[0])));
    // printf("ip: %s\n", ip);

    // Create TCP socket 
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Error in creating socket...\n");
    }

    // Server address to connect
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(ip);

    size_t server_address_length = sizeof(server_address);

    // Establish connection
    int connection_status = connect(client_socket, (struct sockaddr*)&server_address, server_address_length);
    if (connection_status == -1) {
        printf("Error in connecting...\n");
    }

    char request[LIMIT * LIMIT];
    // sprintf(request, "GET / HTTP/1.1\r\nHost: %s \r\n\r\n", ip);
    // sprintf(request, "GET /wp-content/uploads/2021/02/NIRF-2021-SSNCE-ENGG.pdf HTTP/1.1\r\nHost: www.ssn.edu.in\r\n\r\n");
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", arr[0], arr[1]);
    // printf("-----------\n%s\n-----------\n", request);

    // Send request
    if (send(client_socket, request, strlen(request), 0) < 0)
    {
        printf("Error in sending...\n");
        return -1;
    }

    // Accept filename
    char docName[LIMIT];
    printf("Enter the new file name: ");
    scanf("%[^\n]%*c", docName);

    // Store response in a file
    int fd = creat(docName, S_IRWXU);

    char response[LIMIT];
    int bytes;
    while ((bytes = read(client_socket, response, sizeof(response))) > 0)
    {
        write(fd, response, bytes);
        // write(0, response, bytes);
    }

    printf("Downloaded to %s...\n", docName);

    close(fd);
    close(client_socket);

    return 0;

}