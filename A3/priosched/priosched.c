#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <errno.h>

int *status;
int gContinue = 1;

void handleSigInt(int param)
{
    gContinue = 0;
}


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

int getUsage()
{
    puts("Wrong input! Usage: ./priosched [COUNTMAX(>0)] [PROCESSCOUNT(>0)] [NICEVAL 1] ... [NICEVAL N]");
    return -1;  
}

int getNiceUsage()
{
    puts("Nice Value must be between 0 and 19");
    return -1;
}
 
int main(int argc, char *argv[])
{ 
    int i;

    signal(SIGINT, handleSigInt);

    
    //input
    
    if(argc < 3)
    {
        return getUsage();
    }
    
    char *endptr = NULL;
    
    long int countTo = strtol(argv[1], &endptr, 10);
    
    if(*endptr || countTo <= 0)
    {
        return getUsage();
    }
        
    long int proccount =  strtol(argv[2], &endptr, 10);
    
    if(*endptr || argc != (proccount + 3) || proccount <= 0)
    {
        return getUsage();
    }
    
    long int nicevals[proccount];
    
    
    for(i = 3; i < proccount + 3; i++)
    {
        nicevals[i-3] = strtol(argv[i], &endptr, 10);
      
        if(nicevals[i-3] < 0 || nicevals[i-3] > 19)
        {
            return getNiceUsage();
        }
        else if(*endptr)
        {
            return getUsage();
        }
    }
    
    id_t *ids = malloc(proccount * sizeof(id_t));
    for(i = 0; i < proccount; i++)
    {
        ids[i] = 0;
    }
    pid_t pid;
    
    for(i = 0; i < proccount; i++)
    {    
        pid = fork();
   
        if(pid == 0)
        {   
            int which = PRIO_PROCESS;
            ids[i] = getpid();
            setpriority(which, ids[i], (int)nicevals[i]);
            //free(ids);
        
            while(gContinue)
            {
                printf("%s%d%s%f\n", "Child: ", i, "Time: ", counter(countTo));
            }
            free(ids);
        
            exit(EXIT_SUCCESS);
        }
   
        else if(pid < 0)
        {
            //status[i] = -1;
        }     
    }

    for(i = 0; i < proccount; i++)
    {
        int stat;
        while (-1 == waitpid(ids[i], &stat, 0));
    }
    free(ids);
 
   	return 0;
}