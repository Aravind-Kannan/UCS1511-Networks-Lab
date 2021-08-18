#include "common.h"

int exists(server table[], int n, char buffer[])
{
  int i;
  for(i = 0; i < n; i++)
  {
    if(strcmp(table[i].name, buffer) == 0)
      return i;
  }
  return -1;
}

int resolver(server table[], int n, char buffer[])
{
  int i = exists(table, n, buffer);
  // printf("%s %s %d\n", table[i].name, buffer, strcmp(table[i].name, buffer));
  if(i != -1)
  {
    memset(buffer, 0, strlen(buffer));
    for(int j = 0; j < table[i].len; j++)
    {
      strcat(buffer, table[i].ip[j]);
      strcat(buffer, "\n");
    }
  }
  return i;
}

int add_server(server table[], int n, char i_name[], char i_ip[])
{
  strcpy(table[n].name, i_name);
  table[n].len = 1;
  strcpy(table[n].ip[0], i_ip);
  return ++n;
}

void add_ip(server table[], int flag, char i_name[], char i_ip[])
{
  strcpy(table[flag].ip[table[flag].len], i_ip);
  table[flag].len++;
}

void print_table(server table[], int n)
{
  printf("%-20s %-20s\n", "Server Name", "IP Address");
  for(int i = 0; i < n; i++)
  {
    printf("%-20s", table[i].name);
    for(int j = 0; j < table[i].len; j++)
    {
      printf("%s, ", table[i].ip[j]);
    }
    printf("\n");
  }
  printf("\n");
}

int check_duplicate(server table[], int n, char i_ip[])
{
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < table[i].len; j++)
    {
      // printf(">> %s %s %d\n", table[i].ip[j], i_ip, strcmp(table[i].ip[j], i_ip));
      if(strcmp(table[i].ip[j], i_ip) == 0)
      {
        printf("No Duplicates...\nPlease re-enter: ");
        return 1;
      }
    }
  }
  return 0;
}

int main()
{

  server table[LIMIT]; // DNS Table
  int n = 0;            // DNS Table row counter
  char i_name[LIMIT];
  char i_ip[LIMIT];

  // Sample data
  // n = add_server(table, n, "www.yahoo.com", "10.2.45.67");
  // n = add_server(table, n, "www.google.com", "142.89.78.66");

  // Accept input from user
  printf("Enter number of servers: ");
  scanf("%d", &n);
  for(int i = 0; i < n; i++)
  {
    printf("---------Server %d---------\n", i);
    printf("Enter Domain name: ");
    scanf(" %[^\n]%*c", table[i].name);
    printf("Enter number of ips: ");
    scanf(" %d", &table[i].len);
    for(int j = 0; j < table[i].len; j++)
    {
      printf("Enter ip %d: ", j);
      do{
        get_ip(i_ip);
      } while(check_duplicate(table, n, i_ip) == 1);
      strcpy(table[i].ip[j], i_ip);
    }
  }


  print_table(table, n);

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

  char choice[5] = "No";

  int max_clients = 3;
  int client[3] = {0};

  // select uses a timeval to time out
  struct timeval t;
  t.tv_sec = 60;

  // file descriptors store for select
  fd_set readfds;
  int recvfd, port = 0;
  int max = max_clients + 1, flag = 0, len_clients = 0, address_length = 0;

  int state = -1;

  do
  {
    bzero(buffer, sizeof(buffer));

    // clears FD_SET: readfds
    FD_ZERO(&readfds);

    // add server_socket FD to the readfds FD_SET
    FD_SET(server_socket, &readfds);

    // total number of file descriptors
    recvfd =  select(max, &readfds, NULL, NULL, &t);

    for(int i = 0; i < FD_SETSIZE; i++)
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

        printf("%d Client requested for: %s\n", port, buffer);

        // fflush(stdin);

        printf("Do you want to modify table? [Y]es / [N]o :");
        scanf(" %[^\n]%*c", choice);

        // bzero(buffer, sizeof(buffer));

        // printf("Server: ");
        // scanf(" %[^\n]%*c", buffer);

        if(resolver(table, n, buffer) == -1)
          strcpy(buffer, "Not found!");

        sendto(i, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr * )&client_address, address_length);

        FD_CLR(server_socket, &readfds);

// START Extras


        state = -1;

        if(strcmp(choice, "Yes") == 0)
        {
          printf("Enter domain name: ");
          scanf(" %[^\n]%*c", i_name);

          printf("Enter ip address: ");

          do{
            get_ip(i_ip);
          } while(check_duplicate(table, n, i_ip) == 1);

          state = exists(table, n, i_name);

          if(state != -1)
          {
            add_ip(table, state, i_name, i_ip);
            printf("New IP address added...\n");
            print_table(table, n);
          }
          // else
          // {
          //   n = add_server(table, n, i_name, i_ip);
          //   printf("New server added...\n");
          //   print_table(table, n);
          // }
        }

// END Extras

      }
    }
  }while(recvfd != 0) ;


  close(server_socket);

  return 0;
}
