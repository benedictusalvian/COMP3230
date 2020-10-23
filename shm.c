#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    pid_t pid;
    int shmid, status;
    int *a, *b, i;

    // A
    shmid = shmget(IPC_PRIVATE, 10 * sizeof(int), 0666 | IPC_CREAT);

    pid = fork();
    if (pid == 0)
    { // Child
        // B
        b = shmat(shmid, 0, 0);

        for (i = 0; i < 10; i++)
        {
            // C
            kill(getpid(), SIGSTOP);

            printf("\t Child at iteration %d, read b[%d] = %d.\n", i, i, b[i]);
        }
        // D
        shmdt(b);
        exit(0);
    }
    else
    { // Parent
        // E
        a = shmat(shmid, 0, 0);

        sleep(1);
        for (i = 0; i < 10; i++)
        {
            // F
            waitpid(pid, &status, WUNTRACED);

            a[i] = i;
            printf("Parent at iteration %d, writes a[%d] = %d.\n", i, i, a[i]);
            // G
            kill(pid, SIGCONT);
        }
        // H
        wait(NULL);
        shmdt(a);
        shmctl(shmid, IPC_RMID, NULL);
    }
}