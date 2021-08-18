#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcmp(), bzero()
#include <fcntl.h> // open()

#include <sys/types.h>  // socket(), bind(), listen(), accept()
#include <sys/socket.h> // socket(), inet_addr(), bind(), listen(), accept()

#include <netinet/in.h> // inet_addr()
#include <arpa/inet.h>  // htons(), inet_addr()

#include  <unistd.h> // read(), write(), close()

#include <errno.h> // to figure [errno]

#define PORT 9002
#define BUFFER_SIZE 256

int main(int argc, char* argv[])
{
  // Check if args contain IPv4 address
  if(argc < 2)
  {
    printf("Usage: ./server <IPv4-address>\n");
    return -1;
  }

  // create the socket file descriptor
  int sockfd;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0); //SOCK_DGRAM - UDP Connection
  if(sockfd == -1)
  {
    printf("Error in creating socket...\nerrno: %d\n", errno);
    return -1;
  }

  // define the server address
  struct sockaddr_in server_address;
  bzero(&server_address, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr =  inet_addr(argv[1]); // INADDR_ANY
  if(server_address.sin_addr.s_addr == -1)
  {
    printf("IPv4 Address Error...\nerrno: %d\n", errno);
    return -1;
  }

  // bind the socket to the specified IP and port
  if(bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1)
  {
    printf("Binding failed...\nerrno: %d\n", errno);
    return -1;
  }

  // define client address
  struct sockaddr_in client_address;
  bzero(&client_address, sizeof(client_address));
  int len = sizeof(client_address);

  // define buffer and message
  char buffer[BUFFER_SIZE];
  char msg[BUFFER_SIZE];

  while(1){
    // ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
    int n = recvfrom(sockfd, (char*) buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*) &client_address, &len);
    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    printf("Enter your message: ");
    scanf("%[^\n]%*c", msg);

    // ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
    sendto(sockfd, (char*) msg, strlen(msg), MSG_CONFIRM, (struct sockaddr*) &client_address, sizeof(client_address));
    printf("----Message sent----\n");
    if(strcmp(msg, "exit") == 0)
    {
      printf("Server exiting....\n");
      break;
    }
  }

  // close the socket
  close(sockfd);

  return 0;
}
