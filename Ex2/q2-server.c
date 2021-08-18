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

int main(int argc, char* argv[])
{
  // Check if args contain IPv4 address
  if(argc < 2)
  {
    printf("Usage: ./q2-server <IPv4-address>\n");
    return -1;
  }

  // create the server socket
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if(server_socket == -1)
  {
    printf("Error in creating socket...\nerrno: %d\n", errno);
    return -1;
  }

  // define the server address
  struct sockaddr_in server_address;

  // clear the address pointer
  bzero(&server_address, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  // server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_addr.s_addr =  inet_addr(argv[1]);

  if(server_address.sin_addr.s_addr == -1)
  {
    printf("IPv4 Address Error...\nerrno: %d\n", errno);
    return -1;
  }

  // bind the socket to the specified IP and port
  if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1)
  {
    printf("Binding failed...\nerrno: %d\n", errno);
    return -1;
  }

  // listen for socket connection, max 5 in queue
  if(listen(server_socket, 5) == -1)
  {
    printf("Error while listening ...\nerrno: %d\n", errno);
    return -1;
  }

  int client_socket;
  client_socket = accept(server_socket, NULL, NULL);

  if(client_socket == -1)
  {
    printf("Error while accepting socket connection ...\nerrno: %d\n", errno);
    return -1;
  }

  printf("Waiting for client :/ ...\n");

  char buffer[256];

  read(client_socket, &buffer, sizeof(buffer));
  printf("File name received...\n");

  int fd = open(buffer, O_RDONLY, 0);
  if(fd < 1)
  {
    printf("File doesn't exist :( ...\n");
    write(client_socket, NULL, 0);
  }
  else
  {
    int read_bytes = 0;

    while(read_bytes = read(fd, &buffer, sizeof(buffer)))
    {
      write(client_socket, buffer, read_bytes);
      bzero(&buffer, sizeof(buffer));
    }

    printf("File transferred :) ...\n");
    close(fd);
  }

  // close the socket
  close(server_socket);

  return 0;
}
