#include "readwritelock.h"

void rwlock_getInfo(struct rwlock *lock)
{
	printf("WC: %d RC: %d WWC: %d\n", lock->writecount, lock->readcount, lock->writewaitcount);

}

void rwlock_init(struct rwlock *lock)
{
	if(pthread_mutex_init(&(lock->lockmutex), 0) < 0)
	{
		perror("Init of readmutex failed!");
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

    lock->writewaitcount = 0;
	
	//lock = &ret;
	puts("initied");
}

void rwlock_lockRead(struct rwlock *lock)
{   
    if(pthread_mutex_lock(&(lock->lockmutex)) < 0)
    {
    	perror("Mutex lock failed!");
    }
	while(lock->writecount > 0)
	{
    	pthread_cond_wait(&(lock->readcondition), &(lock->lockmutex));
    }
    lock->readcount++; 


    if(pthread_mutex_unlock(&(lock->lockmutex)) < 0)
    {
    	perror("Mutex unlock failed");
    }
}

void rwlock_lockWrite(struct rwlock *lock)
{
	if(pthread_mutex_lock(&(lock->lockmutex)) < 0)
    {
    	perror("Mutex lock failed!");
    }
	lock->writewaitcount++;
	while(lock->readcount > 0 || lock->writecount > 0)
	{
		pthread_cond_wait(&(lock->writecondition), &(lock->lockmutex));
	}
	lock->writewaitcount--; 
	lock->writecount++;
	if(pthread_mutex_unlock(&(lock->lockmutex)) < 0)
    {
    	perror("Mutex unlock failed");
    }
}

void rwlock_unlock(struct rwlock *lock)
{
	//writeunlock
	if(lock->writecount > 0)
	{
		if(pthread_mutex_lock(&(lock->lockmutex)) < 0)
    	{
    		perror("Mutex lock failed!");
    	}


		lock->writecount--;
		if(lock->writewaitcount > 0)
		{
			pthread_cond_signal(&(lock->writecondition));
		}
		else
		{
    		pthread_cond_broadcast(&(lock->readcondition));
		}


		if(pthread_mutex_unlock(&(lock->lockmutex)) < 0)
		{
    		perror("Mutex unlock failed");
    	}
	}
	//readunlock
	else if(lock->readcount > 0)
	{
		if(pthread_mutex_lock(&(lock->lockmutex)) < 0)
    	{
    		perror("Mutex lock failed!");
    	}


		lock->readcount--;
		if(lock->readcount == 0 && lock->writewaitcount > 0)
		{
			pthread_cond_signal(&(lock->writecondition));
		}


		if(pthread_mutex_unlock(&(lock->lockmutex)) < 0)
		{
    		perror("Mutex unlock failed");
    	}
    }
	
}

void rwlock_destroy(struct rwlock *lock)
{
	pthread_mutex_destroy(&(lock->lockmutex));
	pthread_cond_destroy(&(lock->readcondition));
	pthread_cond_destroy(&(lock->writecondition));

}