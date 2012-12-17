#include "readwritelock.h"

struct rwlock rwl;
int i = 0;

void *threadread(void *vargp)
{
	rwlock_lockRead(&rwl);
    printf("%s%d\n", "Value: ", i);
	rwlock_getInfo(&rwl);
	rwlock_unlock(&rwl);
	//rwlock_getInfo(&rwl);
    
    return NULL;
}

void *threadwrite(void *vargp)
{
	rwlock_lockWrite(&rwl);
	printf("%s\n", "Writing.....");
	i = i + 1;
	rwlock_getInfo(&rwl);
	rwlock_unlock(&rwl);

	//rwlock_getInfo(&rwl);

    return NULL;
}

int main(void)
{	
	rwlock_init(&rwl);
    //rwlock_lockRead(&rwl);
    //rwlock_unlock(&rwl);
    
	int j = 0;
	int threadoffset = 10;
	pthread_t *threads = malloc(threadoffset * 2 * sizeof(pthread_t)); 
	long t = 1;

	for(j = 0; j < threadoffset * 2; j++)
	{
		if(j % 2 == 0)
		{
			pthread_create(&threads[j], NULL, threadread, (void *) t);
		}
		else
		{
			pthread_create(&threads[j], NULL, threadwrite, (void *) t);
		}
	}
	
	for(j = 0; j < threadoffset * 2; j++)
	{
        //pthread_detach(threads[j]);
	    pthread_join(threads[j],NULL);
	}

	free(threads);
    //sleep(1);

	//rwlock_destroy(&rwl);
	//pthread_exit(NULL);
    
    exit(EXIT_SUCCESS);
}