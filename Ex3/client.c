#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcmp(), bzero()
#include <fcntl.h> // open()

#include <sys/types.h>  // socket(), connect()
#include <sys/socket.h> // socket(), inet_addr(), connect()

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
    printf("Usage: ./client <IPv4-address>\n");
    return -1;
  }

  // create a socket
  int sockfd;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  // AF_INET - Domain[Protocol Family](IPv4 Internet protocols)
  // SOCK_DGRAM- UDP Socket
  // 0 - Protocol Specification (not a raw socket)
  if(sockfd == -1)
  {
    printf("Error in creating socket...\nerrno: %d\n", errno);
    return -1;
  }

  // specify the address for the socket
  struct sockaddr_in server_address;
  int len;
  bzero(&server_address, sizeof(server_address));

  server_address.sin_family = AF_INET; // same family as our socket
  server_address.sin_port = htons(PORT); // set a port to listen to
  // server_address.sin_addr.s_addr =  INADDR_ANY; // INADDR_ANY = 0.0.0.0
  server_address.sin_addr.s_addr =  inet_addr(argv[1]); // convert a numbers and dots TO byte order
  if(server_address.sin_addr.s_addr == -1)
  {
    printf("IPv4 Address Error...\nerrno: %d\n", errno);
    return -1;
  }

  // define buffer and message
  char buffer[BUFFER_SIZE];
  char msg[BUFFER_SIZE];

  while(1)
  {
    printf("Enter your message: ");
    scanf("%[^\n]%*c", msg);

    sendto(sockfd, (char*) msg, strlen(msg), MSG_CONFIRM, (struct sockaddr*) &server_address, sizeof(server_address));
    printf("----Message sent----\n");

    int n = recvfrom(sockfd, (char*) buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*) &server_address, &len );
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    if(strcmp(msg, "exit") == 0)
    {
      printf("Client exiting....\n");
      break;
    }
  }


  // close the socket
  close(sockfd);

  return 0;
}
