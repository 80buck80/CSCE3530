/* Compilation: gcc -o server server.c
   Execution  : ./server 5000
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
//#include <error.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "request.h"
#include "siteCheck.h"

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, n, i, run, count;
   	struct sockaddr_in serv_addr, cli_addr;
    char buffer[1000000], *returnedResponse;

    FILE *list;//File pointer
    FILE *site;//File pointer



    if(argc < 2)
	{

		printf("\nPort number is missing...\n");
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		//error(EXIT_FAILURE, 0, "ERROR opening socket");
    error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		//error(EXIT_FAILURE, 0, "ERROR binding");
    error("ERROR binding");

	printf("\nServer Started and listening to the port %d\n", portno);
    	listen(sockfd, 5);

	while(1)
	{

		//Connecting with the client
		clilen=sizeof(cli_addr);
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
            error("ERROR on accept");

        printf("\nClient is connected...\n");

        //create file to store requested site names
        list = fopen("list.txt", "a+");

        //set number of sites stored to zero
        count = 0;

        // set to run while until 0
        run = 1;

        while(run)
        {
          printf("\nWaiting on message from client...\n");


          //Receiving the message from the client
          bzero(buffer,1000000);
          n = read(newsockfd,buffer,1000000);

          if(n < 0)
              error("ERROR reading from socket");
          else
          {
            strtok(buffer, "\n");
            printf("\nClient has sent: %s\n", buffer);


            if(strcmp(buffer, "bye\n"))
            {

                //Check list to see if site is in the list
                if(checkList(list, buffer))
                {
                    //Send cached data to client
                }
                else
                {
                    //REQUEST THE SITE
                    returnedResponse = request(buffer);

                    //check if return status is 200
                    if(checkResponse(returnedResponse))
                    {
                        //check if list is full
                        if(count <= 5)
                        {
                            //add requested site to list.txt
                            fprintf(list, "%s", buffer);

                            //  create file to store site information
                            site = fopen(buffer, "w");
                            //  write site information to site
                            fprintf(site, "%s", returnedResponse);

                            //Send site to client
                            bzero(buffer,1000000);
                            //sprintf(buffer, "Connection successfull");
                            n = write(newsockfd, returnedResponse, strlen(returnedResponse));
                        }
                    }
                    else
                    {
                        printf("\n\n\n NOPE \n\n\n");
                    }

                }







            }
            else
            {
              //Sending the message to the client
              bzero(buffer,1000000);
              sprintf(buffer, "Closing the connection, bye!");
              n = write(newsockfd, buffer, strlen(buffer));

              //Closing the connection
              close(newsockfd);
              run = 0; //exit while loop

            }
          }
        }
    }

	return 0;
}
