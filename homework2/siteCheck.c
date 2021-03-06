#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "siteCheck.h"


//checks the response of the HTTP request
bool checkResponse(char *response)
{
    char *ok;

    ok = strstr(response, "200");

    if(ok)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//Looks to see if the site is already cached
bool checkList(FILE *siteList, char *siteName)
{

    char temp[512];

    rewind(siteList);

    while(fgets(temp, 512, siteList))
    {
        if(strstr(temp, siteName))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

}


//Replaces the oldest cached site in files.txt
void replaceOldestSite(FILE *siteList, char *siteName)
{
    char siteArray[5][512];
    char temp[512];
    int i;
    rewind(siteList);

    i = 0;

    while(fscanf(siteList, "%s", siteArray[i]) == 1)
    {

        //printf("ARRAY: %s\n", siteArray[i]);

        i++;

    }

    remove(siteArray[0]);

    strcpy(siteArray[0], siteName);

    puts(siteArray[0]);

    fclose(siteList);
    siteList = fopen("list.txt", "w+");

    for(i = 0; i < 5; i++)
    {
        fprintf(siteList, "%s\n", siteArray[i]);
    }

}
