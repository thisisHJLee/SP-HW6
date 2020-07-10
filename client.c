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
char clientMsg[SIZE], serverMsg[SIZE];
pthread_t clientThread;

void *clientRead(void* v) {
        if ( (fd1 = open(FIFO_ServerToClient, O_RDONLY)) == -1) {
               perror("open failed");
               exit(1);
        }

        while(1) {
                if (read(fd1, serverMsg, SIZE) == -1) {
                        perror("read failed");
                        exit(1);
                }

                printf("\n[SERVER] %s[CLIENT] ", serverMsg);
                fflush(stdout);

                if (strcmp(serverMsg, "quit\n") == 0) {
                        printf("Quit chatting\n");
                        exit(0);
                }
        }
}

void clientWrite() {
        if ((fd2 = open(FIFO_ClientToServer, O_WRONLY)) == -1) {
                perror("open failed");
                exit(1);
        }

        while(1) {
                printf("[CLIENT] "); fflush(stdout);
                fgets(clientMsg, SIZE, stdin);

                if (write(fd2, clientMsg, SIZE) == -1) {
                        perror("write failed");
                        exit(1);
                }

                if (strcmp(clientMsg, "quit\n") == 0) {
                        printf("Quit chatting\n");
                        break;
                }
        }
}

int main() {
        pthread_create(&clientThread, NULL, clientRead, &serverMsg);

        clientWrite();
        return 0;
}
