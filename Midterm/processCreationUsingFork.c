#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    int count = 0; // parentCount = 0
    pid_t pid1, pid2;
    pid1 = fork(); // parentCount = 0, pid1Count = 0
    count++;       // parentCount = 1, pid1Count = 1
    if (pid1 == 0)
    {
        pid2 = fork(); // pid1Count = 1, pid2Count = 1
        count++;       // pid1Count = 2, pid2Count = 2
        if (pid2 == 0)
        {
            count = count + 10; // pid2Count = 12. NOT DISPLAYED.
            exit(0);            // pid2 exits and will never print "Hello world!" and count.
        }
    }
    else
    {
        count++; // parentCount = 2
    }
    printf("Hello world!\n");       // Parent and child process pid1 prints this.
    printf("Count is %d\n", count); // Parent and child process pid1 prints this.
}
