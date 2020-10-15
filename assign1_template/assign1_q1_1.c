#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
	printf("This is the BEGINNING of the program.\n\n");
	if (argc - 1 != 3)
	{
		printf("Error: The number of input integers now is %d. Please input 3 integers.\n", argc - 1);
		return -1;
	} // Don't modify this Error Checking part

	{
		int shmid;
		int *differ;

		int size_data = 3 * sizeof(int);

		shmid = shmget(IPC_PRIVATE, size_data, 0666 | IPC_CREAT);
		printf("Apply %d bytes.\n", size_data);
		differ = shmat(shmid, 0, 0);

		pid_t fpid = fork();

		if (fpid == 0)
		{
			printf("Receive a SIGCONT.\n");
			printf("Child process ID: %d.\n", getpid());
			differ = shmat(shmid, 0, 0);
			differ[2] = differ[0] + differ[1];
			printf("Sum of differences: %d.\n\n", differ[2]);
			return 0;
		}
		else
		{
			kill(fpid, SIGSTOP);
			printf("Parent process ID: %d.\n", getpid());

			for (int i = 0; i < 2; i++)
			{
				differ[i] = atoi(argv[i + 2]) - atoi(argv[i + 1]);
			}
			printf("Differences: %d, %d.\n", differ[0], differ[1]);

			printf("Send a SIGCONT to Process %d.\n\n", fpid);
			kill(fpid, SIGCONT);

			wait(NULL);
			printf("Exited Process ID: %d.", fpid);
		}
	}
	printf("\nThis is the END of the program.\n");
	return 0;
}

// Plase use the following lines to print related information if needed.
/*
sleep(1);  // which you may need for registration of signal handlers in child process, 
               //only allowd at the beginning of the parent process after fork();
printf ("Apply %d bytes.\n",***);
printf("Child process A ID: %d.\n", getpid());
printf("Child process B ID: %d.\n", getpid());
printf("Sum of differences: %d.\n\n", ***);
printf("Send a SIGCONT to Process %d.\n\n", ***); 
printf("Parent process ID: %d.\n");
printf("Differences: %d, %d.\n", ***,***);
printf("Send a SIGCONT to process %d.\n\n", ***);
printf("Exited Process ID: %d.\n", **); 
printf("Receive a SIGCONT.\n"); 
printf("Receive a SIGSTOP.\n"); 


printf("Exited Process ID: %d; Count: %d.\n", **, ***); 
printf("The 3rd argument is larger than the 1st argument.\n");
printf("The 3rd argument is smaller than the 1st argument.\n");
printf("The 3rd argument is equal to the 1st argument.\n");   
*/
