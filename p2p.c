#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#define MAX 1000
#define PORT 12000
char user[50];

int main(int argc, char*argv[])
{
    
    int sock1;
    char buffer[MAX];
    struct sockaddr_in address, addy;
    socklen_t length;
    int panic = 1;


if((sock1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
{
    printf("unable to connect to socket :(");
    EXIT_FAILURE;
}

if(setsockopt(sock1, SOL_SOCKET, SO_BROADCAST, &panic, sizeof panic)==-1)
{
    printf("setsockopt (SO_BROADCAST)");
    exit(1);
}

memset(&address, 0, sizeof address);
address.sin_family = AF_INET;
address.sin_addr.s_addr = htonl(INADDR_ANY);
address.sin_port = htons(PORT);

if(bind(sock1, (struct sockaddr*) &address, sizeof address)==-1)
{
    printf("unable to bind socket :(");
    exit(1);
}

while(1)
{
    memset(buffer, 0, MAX);

    length = sizeof addy;

    if (recvfrom(sock1, buffer, MAX, 0, (struct sockaddr *) &addy, &length) == -1)
    {
        perror("recvfrom");
        exit(1);
    }

    printf("NEW ALERT: %s\n", buffer);
    printf("From: '%s:%d\n", inet_ntoa(addy.sin_addr), ntohs(addy.sin_port));

    for(int i=1;i<=255;i++)
    {
        struct sockaddr_in final;
        memset(&final, 0, sizeof final);
        final.sin_family = AF_INET;
        final.sin_addr.s_addr = inet_addr(" 192.168.239.255");
        final.sin_port = htons(PORT);

        if(strcmp(inet_ntoa(addy.sin_addr), " 192.168.239.255")!=0)
        {
            if(sendto(sock1, buffer, MAX, 0, (const struct sockaddr *) &final, sizeof(final))==-1)
            {
                perror("sendto:");
                exit(1);
            }
        }
    }
}

close(sock1);
return 0;

}