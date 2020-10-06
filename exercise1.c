#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    printf("This is the beginning of the program!\n\n");

    for (int i = 0; i < 2; i++)
    {
        pid_t fpid = fork();
        if (fpid < 0)
            printf("Error in executing fork!");
        else if (fpid == 0)
        {
            exit(0);
        }
        else
        {
            pid_t cpid = wait(&status);
            printf("The exited child process ID now is %d, whose parent process ID is %d.", cpid, getpid());
        }
    }

    printf("This is the END of program!");
    return 0;
}