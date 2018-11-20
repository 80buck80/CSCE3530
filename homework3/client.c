/* Compilation: gcc -o UDPclient UDPclient.c
   Execution  : ./UDPclient <port_number> [eg. port_number = 5000, where port_number is the UDP server port number]
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER "129.120.151.94" //IP address of server
#define BUFLEN 512  //Max length of buffer

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other), portno;
    char buf[BUFLEN];
    char message[BUFLEN];
    char *token;
    char serverMessage[3][32];

    // Client IP, ID and lifetime variables
    char *ip;
    int id;
    int lifetime;


    //Client IP Request String Templates

    char *dhcpRequest =  "%s\n%d\n%d";



    system("clear");
    printf("...This is UDP client...\n\n");

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    portno = atoi(argv[1]); //The port on which to listen for incoming data
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(portno);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    // dhcpDiscover:
    // Set inital ip to 0.0.0.0
    ip = "0.0.0.0";
    // Generate Random Number for ID
    id = rand() % 100 + 1;
    //Set nLifetime
    lifetime = -1;
    //Populate Discover Template into message


    //======================================================================================================
    //DHCP DISCOVER MESSAGE
    //======================================================================================================

    printf("Sending DHCP Discover message...\n");

    sprintf(message, dhcpRequest, ip, id, lifetime);


    //Send message to server
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }

    //======================================================================================================
    //DHCP OFFER MESSAGE
    //======================================================================================================

    //Receiving reply from server and printing it
    //clear the buffer by filling null, it might have previously received data
    bzero(buf, BUFLEN);
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
    {
       die("recvfrom()");
    }

    token = strtok(buf, "\n");
    i = 0;
    while (token != NULL)
    {

      strcpy(serverMessage[i], token);
      i++;
      token = strtok(NULL, "\n");

    }

    printf("\nServer IP Offer:\n");
    printf("yiaddr: %s\nTransaction ID: %s\nLifetime: %s\n", serverMessage[0], serverMessage[1], serverMessage[2]);

    //======================================================================================================
    //DHCP REQUEST MESSAGE
    //======================================================================================================

    printf("Sending DHCP Request message...\n");

    id++;
    sprintf(message, dhcpRequest, serverMessage[0], id, serverMessage[2]);

    //Send message to server
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }

    //======================================================================================================
    //DHCP ACK MESSAGE
    //======================================================================================================


    //Receiving reply from server and printing it
    //clear the buffer by filling null, it might have previously received data
    bzero(buf, BUFLEN);
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
    {
       die("recvfrom()");
    }

    token = strtok(buf, "\n");
    i = 0;
    while (token != NULL)
    {

      strcpy(serverMessage[i], token);
      i++;
      token = strtok(NULL, "\n");

    }

    printf("\nServer ACK message:\n");
    printf("yiaddr: %s\nTransaction ID: %s\nLifetime: %s\n", serverMessage[0], serverMessage[1], serverMessage[2]);


    close(s);
    return 0;
}
