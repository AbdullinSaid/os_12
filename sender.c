#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}  /* External error handling function */
 
int main(int argc, char *argv[])
{
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast address */
    unsigned short broadcastPort;     /* Server port */
    char sendString[100];             /* String to broadcast */
    int broadcastPermission;          /* Socket opt to set permission to broadcast */
    unsigned int sendStringLen;       /* Length of string to broadcast */
 
    if (argc != 2)
    {
        exit(1);
    }
  
    broadcastPort = atoi(argv[1]);
 
    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
 
    /* Set socket to allow broadcast */
    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
          sizeof(broadcastPermission)) < 0)
        DieWithError("setsockopt() failed");
 
    /* Construct local address structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = INADDR_ANY;/* Broadcast IP address */
    broadcastAddr.sin_port = htons(broadcastPort);         /* Broadcast port */
 
    for (;;) /* Run forever */
    {
        if (fgets(sendString, 100, stdin) == NULL) {
            break;
        }
        sendStringLen = strlen(sendString);
         if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *) 
               &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
             DieWithError("sendto() sent a different number of bytes than expected");
 
        sleep(3);
    }
    /* NOT REACHED */
}
