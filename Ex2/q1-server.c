#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcmp(), bzero()

#include <sys/types.h>  // socket(), bind(), listen(), accept()
#include <sys/socket.h> // socket(), inet_addr(), bind(), listen(), accept()

#include <netinet/in.h> // inet_addr()
#include <arpa/inet.h>  // htons(), inet_addr()

#include  <unistd.h> // read(), write(), close()

#define PORT 9002

int main(int argc, char* argv[])
{
  // Check if args contain IPv4 address
  if(argc < 2)
  {
    printf("Usage: ./q1-server <IPv4-address>\n");
    return -1;
  }

  // create the server socket
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // define the server address
  struct sockaddr_in server_address;

  // clear the address pointer
  bzero(&server_address, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  // server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_addr.s_addr =  inet_addr(argv[1]);

  // bind the socket to the specified IP and port
  bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  listen(server_socket, 5);

  int client_socket;
  client_socket = accept(server_socket, NULL, NULL);

  if(client_socket != -1)
  {
    printf("Connection established :) ...\n");
  }

  /*
  char server_message[256] = "This is a server message! Hello!";
  // send the message
  send(client_socket, server_message, sizeof(server_message), 0);
  */

  char client_msg_dest[256];
  // char server_msg_src[256];

  while(1)
  {
    // bzero(server_msg_src, sizeof(server_msg_src));
    read(client_socket, &client_msg_dest, sizeof(client_msg_dest));
    if(strcmp(client_msg_dest, "exit") == 0)
    {
      break;
    }
    printf("Message from client----------\n%s\n", client_msg_dest);

    /*
    printf("Enter your message: ");
    scanf(" %[^;]%*c", server_msg_src);

    if(strcmp(server_msg_src, "exit") == 0)
    {
      break;
    }
    */

    write(client_socket, client_msg_dest, sizeof(client_msg_dest));
  }

  // close the socket
  close(server_socket);

  return 0;
}
