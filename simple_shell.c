#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100

void execute_command(char **tokens);

int main(void)
{
    char input[MAX_INPUT_SIZE];
    char *tokens[MAX_TOKENS];
    pid_t pid;
    int status;

    while (1)
    {
        // Print shell prompt
        printf("$ ");

        // Read input
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
        {
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }

        // Tokenize input
        char *token = strtok(input, " \t\n");
        int i = 0;

        while (token != NULL && i < MAX_TOKENS - 1)
        {
            tokens[i++] = token;
            token = strtok(NULL, " \t\n");
        }

        tokens[i] = NULL;

        // Check for exit command
        if (strcmp(tokens[0], "exit") == 0)
        {
            printf("Exiting shell.\n");
            break;
        }

        // Fork a new process
        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process
            execute_command(tokens);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            waitpid(pid, &status, 0);

            if (WIFEXITED(status))
            {
                printf("Child process exited with status %d\n", WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status))
            {
                printf("Child process terminated by signal %d\n", WTERMSIG(status));
            }
        }
    }

    return 0;
}

void execute_command(char **tokens)
{
    if (execvp(tokens[0], tokens) == -1)
    {
        perror(tokens[0]);
        exit(EXIT_FAILURE);
    }
}

