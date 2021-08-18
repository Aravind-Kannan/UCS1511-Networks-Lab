#include <stdio.h>

#include <string.h>     // strcmp()
#include <strings.h>    // bzero()

#include <sys/types.h>  // socket()
#include <sys/socket.h> // socket()

#include <arpa/inet.h>  // htons()
#include <netinet/in.h> // inet_addr(), INADDR_ANY
#include <unistd.h>     // close()

#include "common.h"

#define PORT 9001
#define LIMIT 256

int main(){

  int client_socket;
  struct sockaddr_in server_address;

  // create client socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket == -1)
  {
    printf("Error in creating socket...\n"); return -1;
  }

  // server address to connect
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  int server_address_length = sizeof(server_address);

  // establish connection
  int connection_status = connect(client_socket, (struct sockaddr *) &server_address, server_address_length);
  if (connection_status == -1) {
    printf("Error in connecting...\n"); return -1;
  }

  struct arp sent_packet, recv_packet;
  char local_ip[LIMIT], local_mac[LIMIT];

  // Local client
  printf("Enter the IP address  : "); get_ip(local_ip);
  printf("Enter the MAC address : "); get_mac(local_mac);

  // Client listen to requests
  while(1)
  {
    bzero(&sent_packet, sizeof(sent_packet));
    bzero(&recv_packet, sizeof(recv_packet));

    // read recvd packet
    read(client_socket, (struct arp*) &recv_packet, sizeof(recv_packet));

    if (recv_packet.flag == 0) // normal request FLAG
    {
      printf("ARP request received:\n");
      print_arp(recv_packet);
    }

    if (recv_packet.flag == -1) // kill FLAG
    {
      break;
    }
    else if (recv_packet.flag == 1) // data sent FLAG :-)
    {
      printf("Received Data packet from: %s\nData: %s\n\n", recv_packet.source_mac, recv_packet.data);
    }
    else if (recv_packet.flag == 0 && strcmp(local_ip, recv_packet.destination_ip) == 0)
    {
      // if recvd packets requests THIS client
      printf("IP Address matches...\n\n");
      strcpy(sent_packet.source_ip, local_ip);
      strcpy(sent_packet.source_mac, local_mac);
      strcpy(sent_packet.destination_ip, recv_packet.source_ip);
      strcpy(sent_packet.destination_mac, recv_packet.source_mac);
      sent_packet.mode = 2; // ARP REPLY
      printf("ARP reply sent:\n");
      print_arp(sent_packet);
    }
    else
    {
      printf("IP Address does not match...\n\n");
      sent_packet.flag = -1;
    }

    write(client_socket, (struct arp*) &sent_packet, sizeof(sent_packet));
  }

  close(client_socket);

	return 0;
}
