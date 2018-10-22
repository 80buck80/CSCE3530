#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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
