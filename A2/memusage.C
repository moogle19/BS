#include "memusage.h"

int gContinue = 1;

void handleSigInt(int )
{
	gContinue = 0;
}

int main(int argc, char** argv)
{
	int m = 0, n = 0, t = 3, p = 0;
	int mc = 1, nc = 1, tc = 1, pc = 1;

	signal(SIGINT, handleSigInt);
	int input = 0;
	if(argc > 1)
	{
		int i = 0;
		for(i = 1; i < argc; i++)
		{
			if(!strcmp(argv[i], "-m") && mc)
			{
				m = 1;
				mc = 0;
			}
			else if(!strcmp(argv[i], "-N") && nc)
			{
				//n = atoi(argv[++i]);
				long int nd = strtol(argv[++i], NULL, 10);
				if(nd == 0L)
				{
					printf("%s\n", "Fail");
				}
				else
				{
					printf("%ld\n", nd);
				}
				nc = 0;
			}
			else if(!strcmp(argv[i], "-T") && tc)
			{
				t = atoi(argv[++i]);
				tc = 0;
			}
			else if(!strcmp(argv[i], "-P") && pc)
			{
				p = atoi(argv[++i]);
				pc = 0;
			}
			else
			{
				printf("%s\n" , "Wrong input! Usage: ./memusage [-m] [-N <num>] [-T <sec>] [-P <pid>]");
				return -1;
			}
		}
	}
	DIR* procdir;
	FILE* cmd;
	//int cmd;
	FILE* stats;
	char* path;
	char buffer[TEXTBUF];
	struct dirent* procpoint;

	long int procs;
	procdir = opendir("/proc");
	if(procdir == NULL)
	{
		perror("Fail!");
		return -1;
	}
	while(procpoint = readdir(procdir))
	{
		//puts(procpoint->d_name);
		if(strtol(procpoint->d_name, NULL, 10) > 0)
		{
			//puts(procpoint->d_name);
			//path = strcat("/proc/", procpoint->d_name);
			//printf("%s\n",path);
			//puts(path);
			cmd = fopen("/proc/32477/cmdline", "r");
			if(cmd == NULL)
			{
				puts("Nicht geöffnet!!!");
			}
			while(! feof(cmd))
			{
				if((fgets(buffer, TEXTBUF, cmd)) != NULL)
				{ 
					puts(buffer);
				}
			}
			fclose(cmd);

		}	
	}
	closedir(procdir);
	while(gContinue)
	{
		//system("clear");
		//system("ls -l /proc/");

		sleep(t);
	}

	//printf("\n%d\n%d\n%d\n%d\n", m, n, t, p);
	//system("clear");
}