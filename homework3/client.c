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

    // Sending the message to the server
    printf("Enter client's message : ");
    gets(message);
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }

    //Receiving reply from server and printing it
    //clear the buffer by filling null, it might have previously received data
    bzero(buf, BUFLEN);
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
    {
       die("recvfrom()");
    }
    printf("Server has sent: %s\n", buf);

    close(s);
    return 0;
}
