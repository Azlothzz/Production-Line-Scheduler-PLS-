#include "scheduler.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int pipefd[2];
    pid_t pid;

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        scheduler_main(pipefd);
    }
    close(pipefd[0]);

    printf("\n   ~~WELCOME TO PLS~~ \n\n");
    char line[120];
    while (true) {
        printf("Please enter: > ");
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = '\0';
        write(pipefd[1], line, strlen(line));
        if (strcmp(line, "exitPLS") == 0) {
            printf(" Bye-bye! \n");
            break;
        }
//        process(line,true);
    }
}