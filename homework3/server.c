/* Compilation: gcc -o UDPserver UDPserver.c
   Execution  : ./UDPserver <port_number> [eg. port_number = 5000, where port_number is the UDP server port number]
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512  //Max length of buffer

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len, portno;
    char buf[BUFLEN], message[1024];

    //Server Response String Template
    char *dhcpRespose = "yiaddr: %s\nTransaction ID: %d\nLifetime: 3600 secs";

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
    portno = atoi(argv[1]); //The port on which to listen for incoming data

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(portno);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    system("clear");
    printf("...This is UDP server...\n\n");

    //keep listening for data
    while(1)
    {
        printf("Waiting for client's message...\n\n");
        fflush(stdout);

        //Receiving data from client
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        //print details of the client/peer and the data received
        printf("Received packet from %s, port number:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Client has sent: %s\n", buf);

        //Sending reply to the client
	bzero(message, 1024);
	printf("Enter server's message:");
	gets(message);
        if (sendto(s, message, strlen(message), 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }

    close(s);
    return 0;
}
