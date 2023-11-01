#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024

int main(void) {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("#cisfun$ ");  /* Display the prompt */
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\n");  /* Handle Ctrl+D (end of file) */
            break;
        }

        input[strcspn(input, "\n")] = '\0';  /* Remove the newline character */

        if (strlen(input) == 0) {
            continue;  /* Ignore empty input lines */
        }

        pid_t child_pid = fork();  /* Create a child process*/
        if (child_pid == -1) {
            perror("fork");
            exit(1);
        }

        if (child_pid == 0) {  /* Child process */
            if (execlp(input, input, NULL) == -1) {
                perror(input);  /* Command not found */
                exit(2);
            }
        } else {  /* Parent process */
            wait(NULL);  /* Wait for the child to finish */
        }
    }

    return 0;
}
