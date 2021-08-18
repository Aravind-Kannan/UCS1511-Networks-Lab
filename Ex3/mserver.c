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
  int server_socket;
  struct sockaddr_in server_address, client_address;
  char buffer[BUFFER_LIMIT];

  if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    printf("Error in creating socket...\n"); return -1;
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(server_socket, (struct sockaddr * ) &server_address, sizeof(server_address)) == -1)
  {
    printf("Error while binding address to socket FD...\n"); return -1;
  }

  int max_clients = 3;
  int client[3] = {0};

  // select uses a timeval to time out
  struct timeval t;
  t.tv_sec = 30;

  // file descriptors store for select
  fd_set readfds;
  int recvfd, port = 0;
  int max = max_clients + 1, flag = 0, len_clients = 0, address_length = 0;

  while(recvfd != 0) {
    bzero(buffer, sizeof(buffer));

    // clears FD_SET: readfds
    FD_ZERO(&readfds);

    // add server_socket FD to the readfds FD_SET
    FD_SET(server_socket, &readfds);

    // total number of file descriptors
    recvfd =  select(max, &readfds, NULL, NULL, &t);

    for(int i = 0; i < max; i++)
    {
      flag = 1;
      if(FD_ISSET(i, &readfds))
      {
        bzero(&client_address, sizeof(client_address));
        address_length = sizeof(client_address);

        recvfrom(i, buffer, sizeof(buffer), 0, (struct sockaddr * ) &client_address, &address_length);
        port = ntohs(client_address.sin_port);

        if(len_clients < max_clients) {
          client[len_clients++] = port;
          flag = len_clients;
        } else {
          flag = -1;
          for(int k = 0; k < max_clients; k++)
          {
            if(client[k] == port)
              flag = k + 1;
          }
        }

        if (flag == -1)
        {
          printf("Max clients reached!\n");
          break;
        }

        printf("Client [%d]: %s\n", port, buffer);

        bzero(buffer, sizeof(buffer));
        printf("Server: ");
        scanf(" %[^\n]%*c", buffer);

        sendto(i, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr * )&client_address, address_length);

        FD_CLR(server_socket, &readfds);
      }
    }
  }


  close(server_socket);

  return 0;
}
