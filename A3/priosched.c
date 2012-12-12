#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/resource.h>

double counter(long int max)
{
	struct timeval start, end;
	long int i = 0, j = 0;
	gettimeofday(&start, NULL);
	for(i = 0; i < max; i++)
	{
		j = j + 1;
	}
	gettimeofday(&end, NULL);

	double diff = ((double)end.tv_sec + ((double)end.tv_usec)/1000000) - (((double)start.tv_sec + ((double)start.tv_usec)/1000000));
	return diff;
}


 int forking(int i)
 {
   	int status;
   	pid_t pid;
      
   	pid = fork ();
   
	if(pid == 0)
   	{	
   		int which = PRIO_PROCESS;
   		id_t id = getpid();
   		setpriority(which, id, i);
		//printf("%s%d%s%d\n", "Child: ", i, "PID: ",getpid());
		//counter(100000000000000);
		while(1)
		{
			printf("%s%d%s%f\n", "Child: ", i, "Time: ", counter(1000000));
		}
    	exit(EXIT_SUCCESS);
   	}
   
   	else if(pid < 0)
   	{
    	 /* The fork failed */
     	//printf("Failed to fork(): %s\n", command);
     	status = -1;
   	}	
   	/*else
     	if(waitpid(pid, &status, 0) != pid)
       		status = -1;  */
  	return status;
}
 
int main(int argc, char *argv[])
{ 
	int i;
 
	/* Loop through argv */
	for(i = 1; i < 10; i++)
    	forking(i);

    sleep(100);
 
   	return 0;
}