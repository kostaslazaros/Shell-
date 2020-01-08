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
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Μετατροπή IPv4 and IPv6 διευθύνσεων από text to δυαδική μορφή
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    // Περίπτωση αποτυχίας σύνδεσης στο socket (μη έγκυρη διέυθυνση)
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    // Ο client στέλνει μήνυμα στον server και περιμένει την απάντηση από τον server
    printf("Connected on server port %i\n", PORT); // Ο client συνδέθηκε με επιτυχία (εκτύπωση αντίστοιχου μηνύματος)
    while(1){
		printf(":> ");
		fflush(NULL);
        memset(buffer, '\0', 1024*sizeof(char));
		// Ανάγνωση μιας εντολής
		if (!fgets(line, 1024, stdin))
			return 0;
        if (strcmp(line, "exit") == 10){
            send(sock, "terminate", strlen("terminate"), 0); // Σε περίπτωση αποσύνδεσης το socket κάνει terminate
			exit(0);
        }
        send(sock , line , strlen(line) , 0 );
        valread = read(sock , buffer, 1024);
        printf("%s\n", buffer );

    }
    return 0;
}