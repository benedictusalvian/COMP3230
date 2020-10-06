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
        printf("Error: The number of input integers now is %d. Please input 3 integers.\n\n", argc - 1);
        return 0;
    }
    int shmid;
    int *shmp, *shmc;

    int size_data = 3 * sizeof(int);

    shmid = shmget(IPC_PRIVATE, size_data, 0666 | IPC_CREAT);

    pid_t fpid = fork();
    if (fpid < 0)
        printf("Error in executing fork!\n");
    else if (fpid == 0)
    {
        shmc = shmat(shmid, 0, 0);
        for (int i = 1; i < 4; i++)
        {
            shmc[i - 1] = atoi(argv[i]);
        }
        qsort(shmc, 3, sizeof(int), compare);
        printf("Child process ID: %d; Sorting results: %d, %d, %d.\n", getpid(), shmc[0], shmc[1], shmc[2]);
        shmdt(shmc);
        return 0;
    }

    else
    {
        wait(NULL);

        shmp = shmat(shmid, 0, 0);
        char num1[11];
        char num2[11];
        sprintf(num1, "%d", shmp[0]);
        sprintf(num2, "%d", shmp[1]);

        printf("Parent process ID: %d; Calculate the sum of the two smallest arguments: %d, %d.\n", getpid(), shmp[0], shmp[1]);
        shmdt(shmp);
        shmctl(shmid, IPC_RMID, NULL);

        char *arg_vec[] = {"./para_sum", num1, num2, NULL};
        execv("./para_sum", arg_vec);
    }
    return 0;
}