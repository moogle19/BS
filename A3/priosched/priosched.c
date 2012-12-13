#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>

int *status;

double counter(long int max)
{
	struct timeval start, end;
	long int i = 0;
	gettimeofday(&start, NULL);
	for(i = 0; i < max; i++)
	gettimeofday(&end, NULL);

	double diff = ((double)end.tv_sec + ((double)end.tv_usec)/1000000) - (((double)start.tv_sec + ((double)start.tv_usec)/1000000));
	return diff;
}


 int forking(int i, long int nvals[], long int countTo)
 {
   	pid_t pid;
      
   	pid = fork ();
   
	if(pid == 0)
   	{	
        int which = PRIO_PROCESS;
   		id_t id = getpid();
   		setpriority(which, id, (int)nvals[i]);
        
		while(1)
		{
			printf("%s%d%s%f\n", "Child: ", i, "Time: ", counter(countTo));
		}
		
    	exit(EXIT_SUCCESS);
   	}
   
   	else if(pid < 0)
   	{
     	status[i] = -1;
   	}	    

  	return status[i];
}
 
int main(int argc, char *argv[])
{ 
	int i;
    
    //input
    
    if(argc < 3)
    {
      puts("Wrong input! Usage: ./priosched [COUNTMAX(>0)] [PROCESSCOUNT(>0)] [NICEVAL 1] ... [NICEVAL N]");
      return -1;
    }
    
    char *endptr = NULL;
    
    long int countTo = strtol(argv[1], &endptr, 10);
    
    if(*endptr || countTo <= 0)
    {
      puts("Wrong input! Usage: ./priosched [COUNTMAX(>0)] [PROCESSCOUNT(>0)] [NICEVAL 1] ... [NICEVAL N]");
      return -1;
    }
        
    long int proccount =  strtol(argv[2], &endptr, 10);
    
    if(*endptr || argc != (proccount + 3) || proccount <= 0)
    {
      puts("Wrong input! Usage: ./priosched [COUNTMAX(>0)] [PROCESSCOUNT(>0)] [NICEVAL 1] ... [NICEVAL N]");
      return -1;
    }
    
    long int nicevals[proccount];
    
    
    for(i = 3; i < proccount + 3; i++)
    {
      nicevals[i-3] = strtol(argv[i], &endptr, 10);
      
      if(nicevals[i-3] < 0 || nicevals[i-3] > 19)
      {
        puts("Nice Value must be between 0 and 19");
        return -1;
      }
      else if(*endptr)
      {
      puts("Wrong input! Usage: ./priosched [COUNTMAX(>0)] [PROCESSCOUNT(>0)] [NICEVAL 1] ... [NICEVAL N]");
        return -1;
      }
    }
    
    status = malloc(sizeof(int) * proccount);
    
	for(i = 0; i < proccount; i++)
	{
       forking(i, nicevals, countTo);
	}
	for(i = 0; i < proccount; i++)
    {
      wait(&(status[i]));
    }
    free(status);
 
   	return 0;
}