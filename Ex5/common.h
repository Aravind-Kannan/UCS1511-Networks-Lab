#include <stdio.h>
#include <stdlib.h>      // atoi()

#include <string.h>     // strcmp()
#include <strings.h>    // bzero()

#include <sys/types.h>  // socket(), sendto(), recvfrom()
#include <sys/socket.h> // socket(), sendto(), recvfrom()
#include <arpa/inet.h>  // htons(), htonl()
#include <netinet/in.h> // INADDR_ANY
#include <unistd.h>     // close()

#define PORT 9001
#define BUFFER_LIMIT 256
#define LIMIT 100

typedef struct server{
  char name[LIMIT];
  char ip[LIMIT][LIMIT];
  int len;
}server;

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
