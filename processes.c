#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int i;

    for (i = 0; i < 5; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("Error in fork");
            return 1;
        }
        else if (pid == 0)
        {
            // Child process
            execlp("ls", "ls", "-l", "/tmp", NULL);
            perror("Error in execlp");
            return 1;
        }
    }

    // Parent process
    for (i = 0; i < 5; i++)
    {
        wait(NULL);
    }

    return 0;
}

