#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    printf("This is the beginning of the program!\n\n");
    int status;
    pid_t rootpid = getpid();

    for (int i = 0; i < 2; i++)
    {
        pid_t fpid = fork();
        if (fpid < 0)
            printf("Error in executing fork!");
        else if (fpid == 0)
        {
            return 0;
        }
        else
        {
            pid_t cpid = wait(&status);
            printf("The exited child process ID now is %d, whose parent process ID is %d.\n", cpid, getpid());
        }
    }

    if (rootpid == getpid())
    {
        printf("The final exited process ID now is %d.\n\n", rootpid);
        printf("This is the END of program!");
    }
    return 0;
}