#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "siteCheck.h"



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

void replaceOldestSite(FILE *siteList, char *siteName)
{
    char *siteArray[4];
    char temp[512];
    int i;
    rewind(siteList);

    i = 0;

    while(fgets(temp, 512, siteList))
    {
        strcpy(siteArray[i], temp);
        i++;
    }

    strcpy(siteArray[0], siteName);

    siteList = fopen("list.txt", "w");

    for(i = 0; i < 5; i++)
    {
        fprintf(siteList, "%s\n", siteArray[i]);
    }

    fclose(siteList);




}
