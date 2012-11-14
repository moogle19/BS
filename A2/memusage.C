#include "memusage.h"

int gContinue = 1;
long int* vmem;

void handleSigInt(int )
{
	gContinue = 0;
}

int sizeCmp(const void* val1, const void* val2)
{
	int* value1 = (int*)val1;
	int* value2 = (int*)val2;
	if(vmem[*value1] < vmem[*value2])
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int main(int argc, char** argv)
{
	long int m = 0, n = 0, t = 3, p = 0;
	int mc = 1, nc = 1, tc = 1, pc = 1;

	//signal interrupt handling
	signal(SIGINT, handleSigInt);

	//check console input
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
					perror("Wrong value for -N (No int) ! Usage: ./memusage [-m] [-N <num>] [-T <sec>] [-P <pid>]");
					return -1;
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
				//t = atoi(argv[++i]);
				long int td = strtol(argv[++i], NULL, 10);
				if(td == 0L)
				{
					perror("Wrong value for -T (No int) ! Usage: ./memusage [-m] [-N <num>] [-T <sec>] [-P <pid>]");
					return -1;
				} //test
				else
				{
					t = td;
					//printf("%ld\n", nd);
				}
				tc = 0;
			}
			else if(!strcmp(argv[i], "-P") && pc)
			{
				//p = atoi(argv[++i]);
				long int pd = strtol(argv[++i], NULL, 10);
				if(pd == 0L)
				{
					perror("Wrong value for -P (No int) ! Usage: ./memusage [-m] [-N <num>] [-T <sec>] [-P <pid>]");
					return -1;
				} //test
				else
				{
					p = pd;
					//printf("%ld\n", nd);
				}
				pc = 0;
			}
			else
			{
				printf("%s\n" , "Wrong input! Usage: ./memusage [-m] [-N <num>] [-T <sec>] [-P <pid>]");
				return -1;
			}
		}
	}


	//directory init
	DIR* procdir;
	struct dirent* procpoint;
	int proccount = 0;

	//output loop
	while(gContinue)
	{	
		//open /proc directory
		procdir = opendir("/proc");
		if(procdir == NULL)
		{
			perror("Failed to open /proc");
			return -1;
		}
		else
		{

			FILE* file;
			proccount = 0;

			//set procdir to start
			rewinddir(procdir);
			
			//get number of running processes
			procpoint = readdir(procdir);
			while(procpoint)
			{
				if(strtol(procpoint->d_name, NULL, 10) > 0)
				{
					proccount++;
				}
				procpoint = readdir(procdir);
			}
			rewinddir(procdir);

			//pointer init for all values
			long int* pid = (long int*)malloc(proccount*sizeof(long int));
			long int* pidpos = pid;

			/*long int*/ vmem = (long int*)malloc(proccount*sizeof(long int));
			long int* vmempos = vmem;

			char* state = (char*)malloc(proccount*(sizeof(char)+1));
			char* statepos = state;

			char** cmd = (char**)malloc(proccount*sizeof(char*));
			char** cmdpos = cmd;

			int cmdcount = 0;
			for(cmdcount = 0; cmdcount < proccount; cmdcount++)
			{
				//initalize all values in case process count between loops is lowered
				*cmd = (char*)malloc(BUFSIZE*sizeof(char));
				strcpy(*cmd, "[No cmd]");

				*pid = 0L;
				*vmem = 0L;
				*state = '-';

				++pid;
				++vmem;
				++state;
				++cmd;
			}
			pid = pidpos;
			vmem = vmempos;
			state = statepos;
			cmd = cmdpos;

			char* buffer = (char*)malloc(BUFSIZE*sizeof(char));

			long int pidtmp = 0L;
			//check if count of running processes has changed
			int checkcount = 0;

			//read the /proc directory
			procpoint = readdir(procdir);
			while(procpoint && checkcount <= proccount)
			{
				//check if directoryname is an integer
				if((pidtmp = strtol(procpoint->d_name, NULL, 10)) > 0)
				{
					*pid = pidtmp; //save pid
					pid++; //increment pid pointer

					//create string with pid for file access
					char* pidstr = (char*)malloc((strlen(procpoint->d_name) + 1)*sizeof(char)); 
					strcpy(pidstr, procpoint->d_name); //save pid as string


					//access status-file
					int pathlength = strlen("/proc/") + strlen("/status") + strlen(pidstr) + 1;

					char* path = (char*)malloc(pathlength*sizeof(char));

					char* tmp;

					//make status-path
					strcpy(path, "/proc/");
					strcat(path, pidstr);
					strcat(path, "/status");

					//file = NULL;
					file = fopen(path, "r"); //open status file
					if(file == NULL)
					{
						printf("%s\n", path);
						perror("Failed to access statusfile!");
						//return -1;
					}
					else
					{

						while(! feof(file))
						{
							if((fgets(buffer, BUFSIZE, file)) != NULL)
							{	
								if(strncmp(buffer, "VmSize:", 7) == 0)
								{
									tmp = strtok(buffer, " ");
									tmp = strtok(NULL, " ");
									*vmem = strtol(tmp, NULL, 10);
									*vmem *= 1024;
								}
								else if(strncmp(buffer, "State", 5) == 0)
								{
									tmp = strtok(buffer, "\t ");
									tmp = strtok(NULL, "\t ");
									*state = tmp[0];
								}
							}
						}
						fclose(file);

					}
					//increment pointer
					++vmem;
					++state;
					//close file and free path
					free(path);

					//access cmd-file
					pathlength = strlen("/proc/") + strlen("/cmdline") + strlen(pidstr) + 1;
					path = (char*)malloc(pathlength*sizeof(char));

					strcpy(path, "/proc/");
					strcat(path, pidstr);
					strcat(path, "/cmdline");

					//file = NULL;
					file = fopen(path, "r");
					if(file == NULL)
					{
						perror("Failed to access cmdfile!");
						strcpy(*cmd, "[CLOSED]");
						//return -1;

					}
					else
					{
						if((fgets(buffer, BUFSIZE, file)) != NULL)
						{ 
							//*cmd = (char*)malloc(BUFSIZE*sizeof(char));
							strcpy(*cmd, buffer);
							//puts(cmdline);
						}
						else
						{	
							//*cmd = (char*)malloc(BUFSIZE*sizeof(char));
							strcpy(*cmd, "[ZOMBIE]");
							//puts(cmdline);
						}
						fclose(file);
					}

					++cmd;


					free(path);
					free(pidstr);
					++checkcount;
				}

				procpoint = readdir(procdir);

			}
			pid = pidpos;
			vmem = vmempos;
			state = statepos;
			cmd = cmdpos;


			int j = 0;	

			int* sorted = (int*)malloc(proccount*sizeof(int));
			int* sortedpos = sorted;

			for(j = 0; j < proccount; j++)
			{
				sorted[j] = j;
			}
			if(m)
			{
				qsort(sorted, proccount, sizeof(int), sizeCmp);
			}



			//set variable for max output
			long int max = proccount;

			//if n is set max output = n
			if(n)
			{
				max = n;
			}

			if(p)
			{
				for(j = 0; j < proccount; j++)
				{
					if(p == pid[j])
					{
						printf("%ld\t%ld\t%c\t%s\n", pid[j], vmem[j], state[j], cmd[j]);
					}
				}

			}
			else
			{
				//print the entrys
				for(j = 0; j < max; j++)
				{
					printf("%ld\t%10ld\t%c\t%s\n", pid[*sorted], vmem[*sorted], state[*sorted], cmd[*sorted]);
					sorted++;
				}
				sorted = sortedpos;
			}


			rewinddir(procdir);

			//free all stuff
			pid = pidpos;
			vmem = vmempos;
			state = statepos;
			cmd = cmdpos;

			char** cmdtmp = cmd;
			for(j = 0; j < proccount; j++)
			{	
				cmdtmp = cmd + 1;
				free(*cmd);
				cmd = cmdtmp;
			}
			cmd = cmdpos;
			free(pid);
			free(vmem);
			free(state);
			free(cmd);
			free(buffer);
			free(sorted);
			closedir(procdir);


			sleep(t);
			system("clear");
		}
	}
}