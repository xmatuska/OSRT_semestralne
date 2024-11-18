#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>

int main() {
    char buffer[1024];

    // Vytvorenie socketu
    int client4_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client4_socket == -1) {
        printf("Nemožno vytvoriť klientovský socket!\n");
        return 1;
    }

    printf("Klientovský socket vytvorený!\n");

    // Nastavenie socketu
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");  // Adresa servera
    server_address.sin_port = htons(3108);

   

    // Pripojenie k serveru
    if (connect(client4_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != 0) {
        printf("Nemožno sa pripojiť k serveru!\n");
        close(client4_socket);
        return 1;
    }

    printf("Pripojenie k serveru úspešné!\n");

    // Merač času
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Prijatie Výsledku od servera
    ssize_t recv_size = recv(client4_socket, buffer, sizeof(buffer), 0);

    gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    if (recv_size == -1) {
        perror("Chyba pri čítaní zo servera");
    } else if (recv_size == 0) {
        printf("\nKlient 4 odpojený");
        close(client4_socket);
    } else {
        double result = atof(buffer);
        printf("\n\nVýsledný obsah valca: %f\n\n", result);
        printf("Čas trvania vypoctu aj so zadavanim parametrov: %f sekúnd\n\n", elapsed_time);
    }

    printf("SOCKET 4 ukončený\n");

    // Uzavretie socketu
    close(client4_socket);

    return 0;
}
