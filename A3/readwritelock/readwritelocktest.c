#include "readwritelock.h"

struct rwlock rwl;
int i = 0;

//thread for read operations
void *threadreadlock(void *vargp)
{
	rwlock_lockRead(&rwl); //read lock
    printf("%s%d\n", "Value: ", i); //read and print value
    rwlock_getInfo(&rwl);
	rwlock_unlock(&rwl); //read unlock
    
    return NULL;
}


//thread for write operations
void *threadwritelock(void *vargp)
{
	rwlock_lockWrite(&rwl); //write lock
	printf("%s\n", "Writing.....");
	i = i + 1; //increase value
    rwlock_getInfo(&rwl);

	rwlock_unlock(&rwl); //write unlock

    return NULL;
}

int main(void)
{	

	//initalize read-write lock
	rwlock_init(&rwl);
    
	int j = 0; //counter

	int threadcount = 170; //numer of threads

	//thread array
	pthread_t *threads = malloc(threadcount * 2 * sizeof(pthread_t));
	
	long t = 1;


	//create pthreads (read && write)
	for(j = 0; j < threadcount * 2; j++)
	{
		if(j % 2 == 0)
		{
			pthread_create(&threads[j], NULL, threadreadlock, (void *) t);
		}
		else
		{
			pthread_create(&threads[j], NULL, threadwritelock, (void *) t);
		}
	}
	

	//join threads
	for(j = 0; j < threadcount * 2; j++)
	{
	    pthread_join(threads[j],NULL);
	}

	//free them all
	free(threads);
    
    exit(EXIT_SUCCESS);
}