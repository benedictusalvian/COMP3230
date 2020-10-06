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
            printf("I am %d. My parent is %d. \n", getpid(), getppid());
        }
        else
        {
            wait(NULL);
        }
    }

    printf("This is the END of program. \n\n");
    return 0;
}