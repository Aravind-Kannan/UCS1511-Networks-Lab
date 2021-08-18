#include <stdio.h>

#include <string.h>     // strcmp()
#include <strings.h>    // bzero()

#include <sys/types.h>  // socket(), sendto(), recvfrom()
#include <sys/socket.h> // socket(), sendto(), recvfrom()
#include <arpa/inet.h>  // htons(), htonl()
#include <netinet/in.h> // INADDR_ANY
#include <unistd.h>     // close()

#define PORT 9001
#define BUFFER_LIMIT 256

int main()
{
  int client_socket;
  struct sockaddr_in address;
  char buffer[BUFFER_LIMIT];

  if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    printf("Error in creating socket...\n"); return -1;
  }

  // clear the pointer
  bzero(&address, sizeof(address));

  // set the address fields
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  address.sin_addr.s_addr = htonl(INADDR_ANY);

  int address_length = sizeof(address);

  while(1)
  {
    // clear buffer
    bzero(buffer, sizeof(buffer));

    // set buffer
    printf("Client: ");
    scanf(" %[^\n]%*c", buffer);

    // send buffer
    sendto(client_socket, buffer, sizeof(buffer), MSG_CONFIRM, (const struct sockaddr * )&address, address_length);

    if(strcmp(buffer, "exit") == 0)
      break;

    // clear buffer
    bzero(buffer, sizeof(buffer));

    // receive buffer
    recvfrom(client_socket, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr * )&address, &address_length);

    // print buffer
    printf("Server: %s\n", buffer);

    if(strcmp(buffer, "exit") == 0)
      break;

  }

  close(client_socket);

  return 0;
}
