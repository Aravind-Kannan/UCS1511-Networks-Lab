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
#define LIMIT 256
