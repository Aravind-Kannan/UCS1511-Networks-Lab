#include <stdio.h>

#include <string.h>     // strcmp()
#include <strings.h>    // bzero()

#include <sys/types.h>  // socket(), bind(), getsockopt()
#include <sys/socket.h> // socket(), bind(), getsockopt()

#include <arpa/inet.h>  // htons()
#include <netinet/in.h> // inet_addr(), INADDR_ANY
#include <unistd.h>     // close()

#include "common.h"

#define PORT 9001
#define LIMIT 256
#define NUMBER_OF_CLIENTS 3

int main(){

  int server_socket, client_socket, identified_client_fd = -1;
  struct sockaddr_in server_address, client_address;
  int server_address_length, client_address_length;
  int client_sockets[NUMBER_OF_CLIENTS];

  // create server socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket == -1)
  {
    printf("Error in creating socket...\n"); return -1;
  }

  // server address to bind
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address_length = sizeof(server_address);

  client_address_length = sizeof(client_address);

  // bind server address
  if (bind(server_socket, (struct sockaddr * ) &server_address, sizeof(server_address)) == -1)
  {
    printf("Error while binding address to socket..\n"); return -1;
  }

  // listen on server_socket
  listen(server_socket, 5);

  printf("Listening for %d clients to connect...\n", NUMBER_OF_CLIENTS);

  // accept NUMBER_OF_CLIENTS
  for(int i = 0; i < NUMBER_OF_CLIENTS; i++) {
    client_socket = accept(server_socket, (struct sockaddr * ) &client_address, &client_address_length);
    printf("[%d]Client %d connection successful...\n", client_socket, i + 1);
    client_sockets[i] = client_socket;
  }

  // packet
  char data[LIMIT];
  struct arp sent_packet, recv_packet;
  bzero(&sent_packet, sizeof(sent_packet));

  // input for arp packet
  printf("Enter details of the packet received:\n");
  printf("Destination IP: "); get_ip(sent_packet.destination_ip);
  printf("Source IP     : "); get_ip(sent_packet.source_ip);
  printf("Source MAC    : "); get_mac(sent_packet.source_mac);
  printf("16 bit data   : "); scanf(" %[^\n]%*c", data);
  strcpy(sent_packet.destination_mac, "00-00-00-00-00-00");
  sent_packet.mode = 1; // ARP REQUEST

  sent_packet.flag = 0; // normal request

  while(1) {

    // broadcast arp request to all clients
    for(int i = 0; i < NUMBER_OF_CLIENTS; i++)
    {
      write(client_sockets[i], (struct arp*) &sent_packet, sizeof(sent_packet));
      read(client_sockets[i], (struct arp*) &recv_packet, sizeof(recv_packet));

      // mark the correct client with matching IP addr
      if (recv_packet.mode == 2)
      {
        identified_client_fd = client_sockets[i];
      }
    }

    // if match, then correct client, send data: flag the client
    if (identified_client_fd != -1)
    {
      printf("CLIENT Identified...Sending data...\n");
      sent_packet.flag = 1; // data send FLAG :-)
      strcpy(sent_packet.data, data);
      write(identified_client_fd , (struct arp*) &sent_packet, sizeof(sent_packet));

      // dummy read
      read(identified_client_fd , (struct arp*) &recv_packet, sizeof(recv_packet));
    }
    else // if no match, quit
    {
      printf("No matching CLIENT...\n");
    }

    // kill clients
    bzero(&sent_packet, sizeof(sent_packet));
    sent_packet.flag = -1; // kill FLAG :-)
    for(int i = 0; i < NUMBER_OF_CLIENTS; i++)
    {
      write(client_sockets[i], (struct arp*) &sent_packet, sizeof(sent_packet));
    }
  }

  close(server_socket);

	return 0;
}
