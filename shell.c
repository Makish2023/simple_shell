#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/**
 * read_line - Read a line of input from the user.
 *
 * Return: The input line.
 */
char *read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

/**
 * tokenize_line - Tokenize a line into separate arguments.
 * @line: The input line to tokenize.
 *
 * Return: An array of tokens.
 */
char **tokenize_line(char *line)
{
    int bufsize = MAX_NUM_TOKENS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += MAX_NUM_TOKENS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}

/**
 * execute_command - Execute a command.
 * @args: Array of command arguments.
 */
void execute_command(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

/**
 * main - Entry point of the shell.
 *
 * Return: Exit status.
 */
int main(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("Shell > ");
        line = read_line();
        args = tokenize_line(line);
        execute_command(args);

        free(line);
        free(args);
    } while (status);

    return EXIT_SUCCESS;
}

