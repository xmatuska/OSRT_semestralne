#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/wait.h>
#include <math.h>

#define M_PI (3.14159265358979323846)

struct Params {
    double radius;
    double height;
    double SP;
    double SPL;
    sem_t semaphore; 
    // semafor na synchronizaciu pristupu k zdielanym datam
};

void *vypocetSP(void *args) {
    struct Params *params = (struct Params *)args;

    // cakanie na povolenie pristupu
    //sem_wait(&params->semaphore);

    params->SP = M_PI * params->radius * params->radius;
    printf("\nratam sp\n");

    // semafor sa uvolni
    //sem_post(&params->semaphore);
    
    pthread_exit(NULL);
}

void *vypocetSPL(void *args) {
    struct Params *params = (struct Params *)args;

    // cakanie na povolenie pristupu
    //sem_wait(&params->semaphore);

    params->SPL = 2 * M_PI * params->radius * params->height;
    printf("\nratam spl\n");
    
    // semafor sa uvolni
    //sem_post(&params->semaphore);
    
    pthread_exit(NULL);
}

int main() {
    char buffer[1024];

    // Vytvorenie socketu
    int client2_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client2_socket == -1) {
        printf("Nemožno vytvoriť klientovský socket!\n");
        return 1;
    }

    printf("Klientovský socket vytvorený!\n");

    // Nastavenie socketu
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(3108);

    // Pripojenie k serveru
    if (connect(client2_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != 0) {
        printf("Nemožno sa pripojiť k serveru!\n");
        close(client2_socket);
        return 1;
    }

    printf("Pripojenie k serveru úspešné!\n");

    // Prijatie polomeru od servera
    recv(client2_socket, buffer, sizeof(buffer), 0);
    if (recv == -1)
        printf("\n cannot read from server");
    else if (recv == 0) {
        printf("\n client 2 odpojeny"); 
        close(client2_socket);
    }
    
    double radius = atof(buffer);
    printf("Prijatý polomer: %f\n", radius);
    
    
    // prijatie vysky od servera
    recv(client2_socket, buffer, sizeof(buffer), 0);
    if (recv == -1)
        printf("\n cannot read from server");
    else if (recv == 0) {
        printf("\n client 2 odpojeny"); 
        close(client2_socket);
    }
    
    double height = atof(buffer);
    printf("Prijatá výška: %f\n", height);

    struct Params params;
    params.radius = radius;
    params.height = height;

    // Inicializacia semaforu kvoli pristupu k shared data
    // a  race conditions.
    sem_init(&params.semaphore, 0, 1); 
    
    //vytvorenie pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1){
        perror("Chyba pri vytvarani potrubia");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if(pid == -1){
        perror("Chyba pri vytvarani forku");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0){
        printf("dosiel som do childa");
        close(pipe_fd[0]);

        sem_wait(&params.semaphore);

        pthread_t threadSP, threadSPL;

        pthread_create(&threadSP, NULL, vypocetSP, (void *)&params);
        pthread_create(&threadSPL, NULL, vypocetSPL, (void *)&params);

        //cakanie na dokoncenie vlakien
        pthread_join(threadSP, NULL);
        pthread_join(threadSPL, NULL);

        // uvolnenie semaforu 
        sem_post(&params.semaphore);

        double rs = params.SP + params.SPL;

        char tmp_str[50];

        sprintf(tmp_str, "%f",rs);

        write(pipe_fd[1], tmp_str, sizeof(tmp_str));

        // uzatvorenie zapisovacieho fd
        close(pipe_fd[1]);

    } else {

        // caka kym sa dokoncia vypocty v childe
        sem_wait(&params.semaphore);

        close(pipe_fd[1]);

        char tmp_str[50];
        
        read(pipe_fd[0], tmp_str, sizeof(tmp_str));

        double vysledok = atof(tmp_str);
        
        // zatvorenie read casti
        close(pipe_fd[0]);
        sem_destroy(&params.semaphore);

        printf("Povrch valca: %.2f\n", vysledok);

        memset(buffer,0,sizeof(buffer));
        sprintf(buffer, "%f", vysledok);
        send(client2_socket, buffer, strlen(buffer), 0);
        printf("\nposielam vysledok %s\n", buffer);
        // cakanie na ukoncienie potomka
        wait(NULL);

    }
    
    printf("KONEC\n");
    
    // Uzavretie socketu
    close(client2_socket);

    return 0;
}