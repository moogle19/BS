#include "readwritelock.h"

struct rwlock *rwl;
int i = 0;

void *thread(void *vargp)
{
	/*puts("test");
	rwlock_lockWrite(rwl);
	puts("test1");
	i = i + 1;
	rwlock_unlock(rwl);*/
	pthread_exit(0);
}

int main(void)
{	
	rwlock_init(rwl);
	int j = 0;
	pthread_t threads[100]; 
	for(j = 0; j < 100; j++)
	{
		pthread_create(&threads[i], NULL, thread, NULL);
        pthread_join(threads[i],NULL); 
	}
	return 1;
}


struct rwlock
{
	pthread_mutex_t readmutex;
	pthread_mutex_t writemutex;
};

void rwlock_init(struct rwlock *lock)
{
	puts("initing");
	if(pthread_mutex_init(&(lock->readmutex), 0) < 0)
	{
		perror("Init of readmutex failed!");
	}
	puts("inint");
	if(pthread_mutex_init(&(lock->writemutex), 0) < 0)
	{
		perror("Init of writemutex failed!");
	}
	puts("initied");
}

void rwlock_lockRead(struct rwlock *lock)
{
	if(pthread_mutex_lock(&(lock->readmutex)) < 0)
	{
		perror("Mutex lock failed!");
	} 
}

void rwlock_lockWrite(struct rwlock *lock)
{
	if(pthread_mutex_lock(&(lock->writemutex)) < 0)
	{
		perror("Mutex lock failed!");
	}
}
void rwlock_unlock(struct rwlock *lock)
{
	pthread_mutex_unlock(&(lock->writemutex));
}