#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>

int gContinue = 1;


void handleSigInt(int )
{
	gContinue = 0;
}


int main(int argc, char const *argv[])
{
	void* mem = NULL;
	if(argc != 7)
	{
		puts("Usage: ./memalloc -M <MAX_SIZE> -S <STEPSIZE> -T <STEPTIME>");
		return -1;
	}

	signal(SIGINT, handleSigInt);


	//values
	long int m = 0;
	long int s = 0;
	long int t = 0;

	//check
	int mc = 1;
	int sc = 1;
	int tc = 1;

	long int tmp;

	int i = 0;
	for(i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-M") && mc)
		{
			tmp = strtol(argv[++i], NULL, 10);
			if(tmp == 0L)
			{
				printf("%s\n", "Wrong input!");
			} //test
			else
			{
				m = tmp;
			}
			mc = 0;
		}
		else if(!strcmp(argv[i], "-S") && sc)
		{
			tmp = strtol(argv[++i], NULL, 10);
			if(tmp == 0L)
			{
				printf("%s\n", "Wrong input");
			} //test
			else
			{
				s = tmp;
			}
			sc = 0;
		}
		else if(!strcmp(argv[i], "-T") && tc)
		{
			tmp = strtol(argv[++i], NULL, 10);
			if(tmp == 0L)
			{
				printf("%s\n", "Wrong input");
			} //test
			else
			{
				t = tmp;
				//printf("%ld\n", nd);
			}
			tc = 0;
		}
		else
		{
			printf("%s\n" , "Wrong input! Usage: ./memusage [-m] [-N <num>] [-T <sec>] [-P <pid>]");
			return -1;
		}
	}
	if(m < s)
	{
		perror("MAX_SIZE is lower than STEPSIZE!");
		return -1;
	}

	

	int steps = 0;
	steps = m/s;
	if(m%s != 0)
	{
		steps++;
	}

	long int alo = s;
	int allallo = 1;
	while(gContinue)
	{
		if(allallo)
		{
			if(alo <= (m-s))
			{
				if(mem != NULL)
				{
					free(mem);
				}
				mem = malloc(alo);
				printf("%s%ld\n", "Allocated memory: ", alo);
				alo+=s;
				sleep(t);
			}
			else
			{
				if(mem != NULL)
				{
					free(mem);
				}
				mem = malloc(m);
				printf("%s%ld\n", "Allocated all memory: ", m);
				allallo = 0;
			}
		}
		else
		{}
		//printf("%ld\t%ld\t%ld\n", m, s, t);
	}
	if(mem != NULL)
	{
		free(mem);
	}
	return 1;
}