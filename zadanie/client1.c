#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <math.h>

int main() {
    char buffer[1024];

    // Vytvorenie socketu
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock_desc == -1) {
        printf("Nemožno vytvoriť socket!\n");
        return 0;
    }

    printf("Socket je vytvorený!\n");

    // Nastavenie socketu
    struct sockaddr_in server;  
    memset(&server, 0, sizeof(server));  
    server.sin_family = AF_INET;  
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  
    server.sin_port = htons(3108);  

    // Pripojenie k serveru
    if (connect(sock_desc, (struct sockaddr*)&server, sizeof(server)) != 0) {
        printf("Nemožno sa pripojiť k serveru!\n");
        close(sock_desc);
        return 0;
    }

    printf("Pripojenie úspešné\n");
    
    // Požiadavka polomeru podstavy od používateľa
    printf("Zadaj polomer podstavy valca: ");
    fgets(buffer, sizeof(buffer), stdin);

    // Poslanie polomeru podstavy na server
    send(sock_desc, buffer, strlen(buffer), 0);

    // Požiadavka výšky valca od používateľa
    printf("Zadaj výšku valca: ");
    fgets(buffer, sizeof(buffer), stdin);

    // Poslanie výšky valca na server
    send(sock_desc, buffer, strlen(buffer), 0);

    // // Prijatie a výpis objemu valca
    // memset(buffer, 0, sizeof(buffer));
    // recv(sock_desc, buffer, sizeof(buffer), 0);
    // printf("Odpoveď od servera: %s\n", buffer);

    printf("\nOdpojenie klienta\n");
    printf("Odpojené od serveru.\n");

    // Uzavretie socketu
    close(sock_desc);

    return 0;
}
