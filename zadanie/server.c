#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main() {
    char buffer[1024];

    // Vytvorenie socketu
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Nemožno vytvoriť serverový socket!\n");
        return 1;
    }

    printf("Serverový socket vytvorený!\n");

    // Nastavenie socketu
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(3108);

    // Pripojenie socketu
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != 0) {
        printf("Nemožno sa pripojiť k serveru!\n");
        close(server_socket);
        return 1;
    }

    printf("Pripojenie k serveru úspešné!\n");

    // Počúvanie na porte
    if (listen(server_socket, 5) != 0) {
        printf("Nemožno počúvať na porte!\n");
        close(server_socket);
        return 1;
    }

    printf("Čakanie na pripojenie klienta...\n");

    // Prijatie pripojenia clienta1
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
        printf("Chyba prijatia pripojenia!\n");
        close(server_socket);
        return 1;
    }

    printf("Pripojenie klienta1 úspešné!\n");

    // Prijatie pripojenia clienta2
    int client2_socket = accept(server_socket, NULL, NULL);
    if (client2_socket < 0) {
        printf("Chyba prijatia pripojenia!\n");
        close(server_socket);
        return 1;
    }

    printf("Pripojenie klienta2 úspešné!\n");

     // Prijatie pripojenia clienta4
    int client4_socket = accept(server_socket, NULL, NULL);
    if (client4_socket < 0) {
        printf("Chyba prijatia pripojenia!\n");
        close(server_socket);
        return 1;
    }

    printf("Pripojenie klienta3 úspešné!\n");



    // Prijatie polomeru podstavy
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    double radius = atof(buffer);

    // Odpoveď klientovi 2
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%f", radius);
    send(client2_socket, buffer, strlen(buffer), 0);
    printf("posielam polomer %s", buffer);

    // Prijatie výšky valca
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    double height = atof(buffer);

    //odpoved klientovi
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%f", height);
    send(client2_socket, buffer, strlen(buffer), 0);
    printf("\nposielam vysku %s\n", buffer);


    //sleep(1000);
    // Prijatie vysledku od CLIENTA2
    memset(buffer, 0, sizeof(buffer));
    recv(client2_socket, buffer, sizeof(buffer), 0);
    double vysledokk = atof(buffer);

    printf("\nDOSTAL SOM VYSLEDOK: %f\n",vysledokk);

    //odpoved klientovi 4 s vysledkom
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%f", vysledokk);
    send(client4_socket, buffer, strlen(buffer), 0);
    printf("\nPOSIELAM VYSLEDOOK: %s\n", buffer);



    // Uzavretie socketov
    //close(client_socket);
    close(server_socket);

    return 0;
}
