#ifndef READWRITELOCK_H
#define READWRITELOCK_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct rwlock;
/*
{
	pthread_mutex_t readmutex;
	pthread_mutex_t writemutex;
};*/


void rwlock_init(struct rwlock *);
void rwlock_lockRead(struct rwlock *);
void rwlock_lockWrite(struct rwlock *);
void rwlock_unlock(struct rwlock *);

#endif /* READWRITELOCK_H */