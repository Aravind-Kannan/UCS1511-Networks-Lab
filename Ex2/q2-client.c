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

char* newFileName(char file[], char addnStr[]);

int main(int argc, char* argv[])
{
  // Check if args contain IPv4 address
  if(argc < 2)
  {
    printf("Usage: ./q2-client <IPv4-address>\n");
    return -1;
  }

  // create a socket
  int network_socket;

  network_socket = socket(AF_INET, SOCK_STREAM, 0);
  // AF_INET - Domain[Protocol Family](IPv4 Internet protocols)
  // SOCK_STREAM - TCP Socket
  // 0 - Protocol Specification (not a raw socket)

  if(network_socket == -1)
  {
    printf("Error in creating socket...\nerrno: %d\n", errno);
    return -1;
  }

  // specify the address for the socket
  struct sockaddr_in server_address;

  // clear the address pointer
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

  // check for error in connection
  if (connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
    printf("Error in connecting to server :/ Try again:(...\n");
    close(network_socket);
    return -1;
  }
  else {
    printf("Connected :) ...\n");
  }

  char buffer[256];
  char fileName[256];
  int bytes, fd;

  bzero(buffer, sizeof(buffer));

  printf("Enter the path of the file: ");
  scanf(" %[^\n]%*c", fileName);
  write(network_socket, fileName, sizeof(fileName));

  bytes = read(network_socket, &buffer, sizeof(buffer));

  if(bytes == 0)
  {
    printf("File doesn't exist :( ... can not find on server...\n");
  }
  else
  {
    fd = open(newFileName(fileName, "_new"), O_CREAT | O_WRONLY, S_IRWXU);
    do
    {
      // printf("\n\nMessage from server----------\n");
      write(fd, &buffer, bytes);
    }while(bytes = read(network_socket, &buffer, sizeof(buffer)));
    close(fd);

    printf("Downloaded to %s\n", newFileName(fileName, "_new"));
  }


  // close the socket
  close(network_socket);

  return 0;
}

char* newFileName(char file[], char addnStr[])
{
  char* fileName;
  fileName = malloc(sizeof(strlen(file) + strlen(addnStr)));

  int j = 0;
  for(int i = 0; i <= strlen(file); i++, j++)
  {
    if(file[i] == '.')
    {
      for(int k = 0; k < strlen(addnStr); k++, j++)
      {
        fileName[j] = addnStr[k];
      }
    }
    fileName[j] = file[i];
  }
  return fileName;
}
