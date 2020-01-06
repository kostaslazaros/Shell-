#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "port.h"
#include "shellfunc.c"

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed"); // Prints system error
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen"); // Prints system error
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    dup2(new_socket, STDOUT_FILENO ); /* duplicate socket on stdout */
    while(1){
        valread = read(new_socket , buffer, 1024);
        // printf("Mesage arrived to server: %s\n", buffer);
        // dup2( new_socket, STDERR_FILENO );  /* duplicate socket on stderr too */
        // setbuf(buffer, NULL);
        // memset(buffer, ' ', 1024*sizeof(char));

        runshell(buffer);
        // memset(buffer, ' ', 1024*sizeof(char));
        // send(new_socket , hello , strlen(hello) , 0 );
        // printf("Message sent back to client\n");
    }
    return 0;
}
