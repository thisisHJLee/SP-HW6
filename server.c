#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define SIZE 128
#define FIFO_ServerToClient "fifo1"
#define FIFO_ClientToServer "fifo2"

int fd1, fd2;
char serverMsg[SIZE], clientMsg[SIZE];
pthread_t serverThread;

void makeFIFO() {
        if ( mkfifo(FIFO_ServerToClient, 0666) == -1 ) {
                perror("mkfifo failed");
                exit(1);
        }

        if ( mkfifo(FIFO_ClientToServer, 0666) == -1 ) {
                perror("mkfifo failed");
                exit(1);
        }
}

void *serverRead(void* v) {
        if ( (fd2 = open(FIFO_ClientToServer, O_RDONLY)) == -1 ) {
                perror("open failed");
                exit(1);
        }

        while(1) {
                if (read(fd2, clientMsg, SIZE) == -1) {
                        perror("read failed");
                        exit(1);
                }

                printf("\n[CLIENT] %s[SERVER] ", clientMsg);
                fflush(stdout);

                if (strcmp(clientMsg, "quit\n") == 0) {
                        printf("Quit chatting\n");
                        exit(0);
                }
        }
}

void serverWrite() {
        if ( (fd1 = open(FIFO_ServerToClient, O_WRONLY)) == -1 ) {
                perror("open failed");
                exit(1);
        }

        while(1) {
                printf("[SERVER] "); fflush(stdout);
                fgets(serverMsg, SIZE, stdin);

                if (write(fd1, serverMsg, SIZE) == -1) {
                        perror("write failed");
                        exit(1);
                }

                if (strcmp(serverMsg, "quit\n") == 0) {
                        printf("Quit chatting\n");
                        break;
                }
        }
}

int main() {
        makeFIFO();

        pthread_create(&serverThread, NULL, serverRead, &clientMsg);

        serverWrite();
        return 0;
}
