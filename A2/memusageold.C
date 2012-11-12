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
				} //test
				else
				{
					n = nd;
					//printf("%ld\n", nd);
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
	FILE* status;
	int read = 1;
	int outcount = 0;

	//TODO: dynamic alloc
	char path[64];
	
	char cmdpath[64];
	char statuspath[64];
	
	char cmdline[TEXTBUF];
	char statusline[TEXTBUF];
	char buffer[TEXTBUF];
	char vmsize[TEXTBUF];
	char state[TEXTBUF];
	
	char* tmp;
	struct dirent* procpoint;

	long int procs;
	/******************************************************/

	int proccount = 0;

	while(gContinue)
	{
		outcount = 0;
		read = 1;
		proccount = 0; //number of running process


		long int procnr;
		procdir = opendir("/proc");
		if(procdir == NULL)
		{
			perror("Failed to open /proc!");
			return -1;
		}
		//get number of active processes
		while(procpoint = readdir(procdir))
		{
			if(strtol(procpoint->d_name, NULL, 10) > 0)
			{
				proccount++;
			}
		}
		rewinddir(procdir);
		long int* procnums = (long int*)malloc(proccount*sizeof(long int)); //list with running processes
		long int* temp = procnums;

		while(procpoint = readdir(procdir))
		{
			if((procnr = strtol(procpoint->d_name, NULL, 10)) > 0)
			{
				*procnums = procnr;
				procnums++;
			}
		}
		procnums = temp;
		/*int i = 0;
		for(i = 0; i < proccount; i++)
		{
			printf("%d\n", procnums[i]);
		}*/


		while((procpoint = readdir(procdir)) && read)
		{
			//puts(procpoint->d_name);
			if(strtol(procpoint->d_name, NULL, 10) > 0)
			{
				
				//get path
				strcpy(path, "/proc/");
				strcat(path, procpoint->d_name);
				strcpy(cmdpath, path);
				strcpy(statuspath, path);

				//cmdline
				strcat(cmdpath, "/cmdline");
				
				//open cmdline file
				cmd = fopen(cmdpath, "r");
				if(cmd == NULL)
				{
					puts("Nicht geöffnet!!!");
				}
				while(! feof(cmd))
				{
					if((fgets(buffer, TEXTBUF, cmd)) != NULL)
					{ 
						strcpy(cmdline, buffer);
						//puts(cmdline);
					}
					else
					{
						strcpy(cmdline, "[ZOMBIE]");
						//puts(cmdline);
					}
				}
				fclose(cmd);

				//other stats

				strcat(statuspath, "/status");

				status = fopen(statuspath, "r");
				if(status == NULL)
				{
					fprintf(stderr, "%s\n", "Error in status path!");
					return -1;
				}
				while(! feof(status))
				{
					if((fgets(buffer, TEXTBUF, status)) != NULL)
					{
						strcpy(statusline, buffer);
						if(strncmp(statusline, "VmSize:", 5) == 0)
						{
							tmp = strtok(statusline, " ");
							tmp = strtok(NULL, " ");
							strcpy(vmsize, tmp);
						}
						else if(strncmp(statusline, "State", 5) == 0)
						{
							tmp = strtok(statusline, "\t ");
							tmp = strtok(NULL, "\t ");
							strcpy(state, tmp);
							//puts(state);
							//state = strtok(NULL, " ");
						}
					}
				}
				fclose(status);
				/*printf("%s\n", procpoint->d_name);
				printf("%s\n", state);
				printf("%s\n", vmsize);
				printf("%s\n", cmdline);*/

				printf("%s\t%s\t%s\t%s\n", procpoint->d_name, state, vmsize, cmdline);
				outcount++;

			}	
			//printf("%d\n", n);
			if(n && outcount == n)
			{
				read = 0;
			}
		}
		sleep(t);
		free(procnums);
		closedir(procdir);
		system("clear");
	}
	


	//printf("\n%d\n%d\n%d\n%d\n", m, n, t, p);
	//system("clear");
}