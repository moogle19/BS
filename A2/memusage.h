#ifndef memusage_h
#define memusage_h
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>

#define BUFSIZE 1024

void handleSigInt(int );

int sizeCmp(const void*, const void*);

#endif
