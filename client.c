#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "port.h"
static char line[1024];

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Geia kai xara sas";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    // If connection to the socket (adress is invalid print an error message and just exit)
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    // Otherwise send the client message and wait to reveive the server message
    while(1){
		printf("~> ");
		fflush(NULL);
        memset(buffer, '\0', 1024*sizeof(char));
		/* Read a command line */
		if (!fgets(line, 1024, stdin))
			return 0;
        if (strcmp(line, "exit") == 10)
			exit(0);
        // printf("Sygkrisi: %i", strcmp(line, "exit"));
        send(sock , line , strlen(line) , 0 );
        // printf("Message sent to server: %s\n", line);
        valread = read(sock , buffer, 1024);
        printf("%s\n", buffer );

    }
    return 0;
}