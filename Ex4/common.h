#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT 256

struct arp {
  int mode;
  char destination_ip[LIMIT], destination_mac[LIMIT];
  char source_ip[LIMIT], source_mac[LIMIT];
  char data[LIMIT];
  int flag;
};

void print_arp(struct arp packet){
  printf("%d|%s|%s|%s|%s\n", packet.mode, packet.source_mac, packet.source_ip, packet.destination_mac, packet.destination_ip);
}

int check_ip(char ip[])
{
  char substr[LIMIT];
  int substr_index = 0, temp, counter = 0;

  for(int i=0; i <= strlen(ip); i++)
  {
    if(ip[i] == '.' || ip[i] == '\0')
    {
      substr[substr_index] = '\0';
      temp = atoi(substr);
      // printf("substr=%s temp=%d\n", substr, temp);
      if(temp < 0 || temp > 255)
      {
        printf("Error in range: Between 0 and 255\nPlease re-enter: ");
        return 0;
      }
      substr_index = 0;
      counter++;
    }
    else
    {
      substr[substr_index++] = ip[i];
    }
  }

  if(counter != 4)
  {
    printf("Error in format: [0-255].[0-255].[0-255].[0-255]\nPlease re-enter: ");
    return 0;
  }
  return 1;
}

void get_ip(char ip[])
{
  do
  {
    scanf(" %[^\n]%*c", ip);
  }
  while(check_ip(ip) == 0);
}

int check_mac(char mac[])
{
  char substr[LIMIT];
  int ind = 0, temp, counter = 0;

  for(int i=0; i <= strlen(mac); i++)
  {
    if(mac[i] == '-' || mac[i] == '\0')
    {
      if(ind != 2)
      {
        printf("Error in range: Always in 2 bits (00 to FF)\nPlease re-enter: ");
        return 0;
      }
      ind = 0;
      counter++;
    }
    else
    {
      substr[ind] = mac[i];
      //printf("%c %d %d\n", mac[i], mac[i], mac[i] <= '9');
      if( (mac[i] < '0' || mac[i] > '9') && (mac[i] < 'A' || mac[i] > 'F') && (mac[i] < 'a' || mac[i] > 'f') )
      {
        printf("%d %d %d\n", mac[i], '0', mac[i] <= '9');
        printf("Error in range: Between 00 and FF\nPlease re-enter: ");
        return 0;
      }
      ind++;
    }
  }

  if(counter != 6)
  {
    printf("Error in format: [00-FF]-[00-FF]-[00-FF]-[00-FF]-[00-FF]-[00-FF]\nPlease re-enter: ");
    return 0;
  }
  return 1;
}

void get_mac(char mac[])
{
  do
  {
    scanf(" %[^\n]%*c", mac);
  }while(check_mac(mac) == 0);
}
