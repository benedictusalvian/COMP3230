#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc,char* argv[])
{
	printf("Replacing the program by this one!\n");
	// paras should be intergers
		
	if(argc-1>3)
	{
		printf("The number of paras is %d.\n",argc-1);
		printf("Error: The largest number of paras is 3.\n\n");
		return -1;
	}

	int sum=0;
	for(int i=1;i<argc;i++)
	{
		printf("Para %d is %s.\n",i,argv[i]);
		sum+=atoi(argv[i]);
	}
	
	printf("The sum of total paras now is %d.\n\n",sum);
	return sum;
}
