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

char* getIP(int *count);
void parseClient(char *message, char (*arr)[32]);

int main(int argc, char *argv[])
{
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len, portno, ipCount = 10;
    char buf[BUFLEN], message[1024];

    //Server IP and ID variables
    char *ip;
    int id;
    // Token variable used when splitting strings
    char *token;
    char clientMessage[3][32];
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
    while(ipCount > 0)
    {

        printf("\nWaiting for new client DHCP Discover message...\n");
        fflush(stdout);

        //======================================================================================================
        //DHCP DISCOVER MESSAGE
        //======================================================================================================

        //Receiving data from client
        bzero(buf, BUFLEN);
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        //Initialize ipArray
        for(i = 0; i < 10 ; i++)
        {
          strcpy(clientMessage[i], "");
        }


        //print details of the client/peer and the data received
        token = strtok(buf, "\n");
        i = 0;
        while (token != NULL)
        {
          strcpy(clientMessage[i], token);
          i++;
          token = strtok(NULL, "\n");

        }

        printf("\nClient's DHCP Discover message:\n");
        printf("yiaddr: %s\nTransaction ID: %s\n", clientMessage[0], clientMessage[1]);

        //======================================================================================================
        //DHCP OFFER MESSAGE
        //======================================================================================================

        ip = getIP(&ipCount);

        printf("\nChosen IP Address: \n%s\n", ip);

        //Create reply to the client
	      bzero(message, 1024);
        strcpy(message, ip);
        strcat(message, "\n");
        strcat(message, clientMessage[1]);
        strcat(message, "\n");
        strcat(message, "3600\n");

        //Send reply to client
	      printf("\nSending Client an IP offer...\n");
        if (sendto(s, message, strlen(message), 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }

        //======================================================================================================
        //DHCP REQUEST MESSAGE
        //======================================================================================================

        printf("\nWaiting for Client DHCP request...\n");

        //Receiving data from client
        bzero(buf, BUFLEN);
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        //print details of the client/peer and the data received
        token = strtok(buf, "\n");
        i = 0;
        while (token != NULL)
        {
          strcpy(clientMessage[i], token);
          i++;
          token = strtok(NULL, "\n");

        }

        printf("\nClient's DHCP request:\n");
        printf("yiaddr: %s\nTransaction ID: %s\nLifetime: %s\n", clientMessage[0], clientMessage[1], "3600");

        //======================================================================================================
        //DHCP ACK MESSAGE
        //======================================================================================================

        //Create reply to the client
	      bzero(message, 1024);
        strcpy(message, ip);
        strcat(message, "\n");
        strcat(message, clientMessage[1]);
        strcat(message, "\n");
        strcat(message, "3600\n");

        //Send reply to client
	      printf("\nSending Client ACK message...\n");
        if (sendto(s, message, strlen(message), 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }

        printf("IP COUNT:%d", ipCount);
    }

    close(s);
    return 0;
}

char* getIP(int *count)
{
    char ipArray[10][512]; //  Holds ip addresses read in from ip.txt
    char *ip;
    int i; //  Counter
    FILE *fptr;  //  File pointer

    //Initialize ipArray
    // for(i = 0; i < 10 ; i++)
    // {
    //   ipArray[i] = NULL;
    // }

    //Open the ip.txt file
    fptr = fopen("ip.txt", "r");

    i = 0;

    //Read in ip addresses
    while(fscanf(fptr, "%s", ipArray[i]) == 1)
    {
        //printf("IP: %s\n", ipArray[i]);
        i++;
    }

    //Close file pointer to ip.txt
    fclose(fptr);

    //Open and empty ip.txt
    fptr = fopen("ip.txt", "w+");

    //Write IPs to file
    for(i = 1; i < *count; i++)
    {
        fprintf(fptr, "%s\n", ipArray[i]);
    }

    //Close file pointer to ip.txt
    fclose(fptr);

    *count--;//Decrement number of ip addresses left

    ip = ipArray[0];

    return ip; // Return selected IP address
}
