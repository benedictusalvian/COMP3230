#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    int count = 0;
    pid_t pid1, pid2;
    pid1 = fork();
    count++;
    if (pid1 == 0)
    {
        pid2 = fork();
        count++;
        if (pid2 == 0)
        {
            count = count + 10;
            print("I am pid2, count is %d\n", count);
            exit(0);
        }
    }
    else
    {
        count++;
    }
    printf("Hello world!\n");
    printf("Count is %d\n", count);
}