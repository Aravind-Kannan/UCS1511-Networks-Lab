#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcmp(), bzero()

#include <sys/types.h>  // socket(), connect()
#include <sys/socket.h> // socket(), inet_addr(), connect()

#include <netinet/in.h> // inet_addr()
#include <arpa/inet.h>  // htons(), inet_addr()

#include  <unistd.h> // read(), write(), close()

#define PORT 9002

int main(int argc, char* argv[])
{
  // Check if args contain IPv4 address
  if(argc < 2)
  {
    printf("Usage: ./q1-client <IPv4-address>\n");
    return -1;
  }

  // create a socket
  int network_socket;

  network_socket = socket(AF_INET, SOCK_STREAM, 0);
  // AF_INET - Domain[Protocol Family](IPv4 Internet protocols)
  // SOCK_STREAM - TCP Socket
  // 0 - Protocol Specification (not a raw socket)


  // specify the address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET; // same family as our socket
  server_address.sin_port = htons(PORT); // set a port to listen to
  // server_address.sin_addr.s_addr =  INADDR_ANY; // INADDR_ANY = 0.0.0.0
  server_address.sin_addr.s_addr =  inet_addr(argv[1]); // convert a numbers and dots TO byte order

  int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
  // check for error in connection
  if (connection_status == -1) {
    printf("Error in connecting to server :/ Try again:(...\n");
  }
  else {
    printf("Connected :) ...\n");
  }

  /*
  // receive data from server
  char server_response[256];
  recv(network_socket, &server_response, sizeof(server_response), 0);
  // print out response
  printf("Response: \n %s", server_response);
  */

  char client_msg_src[256];
  char server_msg_dest[256];

  while(1)
  {
    bzero(client_msg_src, sizeof(client_msg_src));
    printf("Enter your message: ");
    scanf(" %[^;]%*c", client_msg_src);
    write(network_socket, client_msg_src, sizeof(client_msg_src));
    if(strcmp(client_msg_src, "exit") == 0)
    {
      break;
    }

    read(network_socket, &server_msg_dest, sizeof(server_msg_dest));
    printf("Message from server----------\n%s\n", server_msg_dest);
  }

  // close the socket
  close(network_socket);

  return 0;
}
