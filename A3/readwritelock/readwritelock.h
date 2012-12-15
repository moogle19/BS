#ifndef READWRITELOCK_H
#define READWRITELOCK_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct rwlock
{
	pthread_mutex_t lockmutex;
	pthread_cond_t writecondition;
	pthread_cond_t readcondition;
    int readcount;
    int writecount;
    int writewaitcount;
};


void rwlock_init(struct rwlock *);
void rwlock_lockRead(struct rwlock *);
void rwlock_lockWrite(struct rwlock *);
void rwlock_unlock(struct rwlock *);
void rwlock_getInfo(struct rwlock *);
void rwlock_destroy(struct rwlock *);


#endif /* READWRITELOCK_H */