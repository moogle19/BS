#include "readwritelock.h"

struct rwlock rwl;
int i = 0;

void *thread(void *vargp)
{
	rwlock_lockWrite(&rwl);
	int k = 0;
	for(k = 0; k < 100; k++)
    {
		i = i + 1;
	}
	rwlock_unlock(&rwl);
    
	printf("%d\n", i);
	return NULL;
}

int main(void)
{	
	rwlock_init(&rwl);
    rwlock_lockRead(&rwl);
    rwlock_unlock(&rwl);
    
	int j = 0;
	pthread_t threads[100]; 
	int thread_args[100];
	for(j = 0; j < 100; j++)
	{
		pthread_create(&threads[j], NULL, thread, &thread_args[j]);
	}
	for(j = 0; j < 100; j++)
	{
	    pthread_join(threads[j],NULL); 
	}
    return 1;
}


struct rwlock
{
	pthread_mutex_t readmutex;
	pthread_mutex_t writemutex;
	pthread_cond_t writecondition;
	pthread_cond_t readcondition;
    int readcount;
    int writecount;
};

void rwlock_init(struct rwlock *lock)
{
	if(pthread_mutex_init(&(lock->readmutex), 0) < 0)
	{
		perror("Init of readmutex failed!");
	}
	
	if(pthread_mutex_init(&(lock->writemutex), 0) < 0)
	{
		perror("Init of writemutex failed!");
	}
	
	if(pthread_cond_init(&(lock->writecondition), 0) < 0)
	{
		perror("Init of writecondition failed!");
	}
	
	if(pthread_cond_init(&(lock->readcondition), 0) < 0)
	{
		perror("Init of readcondition failed!");
	}
	
	lock->readcount = 0;
    
    lock->writecount = 0;
	
	//lock = &ret;
	puts("initied");
}

void rwlock_lockRead(struct rwlock *lock)
{   
    (lock->readcount)++;
	if(pthread_mutex_lock(&(lock->readmutex)) < 0)
	{
		perror("Mutex lock failed!");
	} 
}

void rwlock_lockWrite(struct rwlock *lock)
{
    (lock->writecount)++;
	if(pthread_mutex_lock(&(lock->writemutex)) < 0)
	{
		perror("Mutex lock failed!");
	}
}
void rwlock_unlock(struct rwlock *lock)
{
	pthread_mutex_unlock(&(lock->writemutex));
}