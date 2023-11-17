#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

void display_prompt(void);
char *read_command(void);
void execute_command(char *command);

int main(void)
{
    char *command;

    while (1)
    {
        display_prompt();
        command = read_command();

        if (command == NULL)
        {
            printf("\n");  /* Handle Ctrl+D (end of file) */
            break;
        }

        if (strcmp(command, "exit") == 0)
        {
            free(command);
            break;
        }

        execute_command(command);

        free(command);
    }

    return 0;
}

void display_prompt(void)
{
    printf("#cisfun$ ");
}

char *read_command(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;

    if (getline(&buffer, &bufsize, stdin) == -1)
    {
        free(buffer);  /* Free allocated memory */
        return NULL;
    }

    /* Remove newline character */
    buffer[strcspn(buffer, "\n")] = '\0';

    return buffer;
}

void execute_command(char *command)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /* Child process */
        char *args[] = {command, NULL};
        if (execve(command, args, NULL) == -1)
        {
            fprintf(stderr, "%s: command not found\n", command);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */
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

