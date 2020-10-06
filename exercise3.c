#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[])
{
    if (argc - 1 != 3)
    {
        printf("Error: The number of input integers now is %d. Please input 3 integers.", argc - 1);
        return 0;
    }
    int shmid;
    int *shmp, *shmc;

    int size_data = 3 * sizeof(int);

    shmid = shmget(IPC_PRIVATE, size_data, 0666 | IPC_CREAT);

    pid_t fpid = fork();
    if (fpid < 0)
        printf("Error in executing fork!");
    else if (fpid == 0)
    {
        shmc = shmat(shmid, 0, 0);
        for (int i = 1; i < 4; i++)
        {
            shmc[i - 1] = atoi(argv[i]);
        }
        qsort(shmc, 3, sizeof(int), compare);
        printf("%d %d %d", shmc[0], shmc[1], shmc[2]);
        return 0;
    }
    else
    {
        wait(NULL);
    }
}