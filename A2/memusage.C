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

	//directory init
	DIR* procdir;
	struct dirent* procpoint;
	int proccount = 0;

	while(gContinue)
	{	

		procdir = opendir("/proc");
		if(procdir == NULL)
		{
			perror("Failed to open /proc");
			return -1;
		}

		FILE* file;
		proccount = 0;

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

		//pointer for all values
		long int* pid = (long int*)malloc(proccount*sizeof(long int));
		long int* pidpos = pid;

		long int* vmem = (long int*)malloc(proccount*sizeof(long int));
		long int* vmempos = vmem;

		char* state = (char*)malloc(proccount*sizeof(char));
		char* statepos = state;

		char** cmd = (char**)malloc(proccount*sizeof(char*));
		char** cmdpos = cmd;

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
				pid++;

				char* pidstr = (char*)malloc(strlen(procpoint->d_name)*sizeof(char)); 
				strcpy(pidstr, procpoint->d_name); //save pid as string


				//access status-file
				int pathlength = strlen(PROCPATH) + strlen(STATUS) + strlen(pidstr);

				char* path = (char*)malloc(pathlength*sizeof(char));

				char* tmp;

				//make status-path
				strcpy(path, PROCPATH);
				strcat(path, pidstr);
				strcat(path, STATUS);

				file = fopen(path, "r"); //open status file
				if(file == NULL)
				{
					perror("Failed to access statusfile!");
				}

				*vmem = 0L; //set vmem to zero in case proccess has no vmsize entry
				while(! feof(file))
				{
					if((fgets(buffer, BUFSIZE, file)) != NULL)
					{	
						//strcpy(statusline, buffer);
						if(strncmp(buffer, "VmSize:", 7) == 0)
						{
							tmp = strtok(buffer, " ");
							tmp = strtok(NULL, " ");
							*vmem = strtol(tmp, NULL, 10);
						}
						else if(strncmp(buffer, "State", 5) == 0)
						{
							tmp = strtok(buffer, "\t ");
							tmp = strtok(NULL, "\t ");
							*state = tmp[0];
						}
					}
				}
				//increment pointer
				++vmem;
				++state;

				//close file and free path
				fclose(file);
				free(path);

				//access cmd-file
				pathlength = strlen(PROCPATH) + strlen(CMD) + strlen(pidstr);
				path = (char*)malloc(pathlength*sizeof(char));

				strcpy(path, PROCPATH);
				strcat(path, pidstr);
				strcat(path, CMD);


				file = fopen(path, "r");
				if(file == NULL)
				{
					perror("Failed to access cmdfile!");
				}
				*cmd = (char*)malloc(BUFSIZE*sizeof(char));

	
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

				++cmd;

				fclose(file);

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


		int i = 0;
		int j = 0;

		//sort size
		long int minval = 0L;
		long int oldminval = 0L;
		int min = 0;

		minval = vmem[0];
		for(j = 1; j < proccount; j++)
		{
			if(j != min && vmem[i] < minval)
			{
				minval = vmem[i];
				min = j;
			}
		}

		//set variable for max output
		long int max = proccount;

		//if n is set max output = n
		if(n)
		{
			max = n;
		}

		//print the entrys
		for(i = 0; i < max; i++)
		{
			printf("%ld\t%ld\t%c\t%s\n", *pid, *vmem, *state, *cmd);
			pid++;
			vmem++;
			state++;
			cmd++;
		}


		rewinddir(procdir);

		//free all stuff
		pid = pidpos;
		vmem = vmempos;
		state = statepos;
		cmd = cmdpos;

		for(i = 0; i < proccount; i++)
		{	
			free(*cmd);
			cmd++;
		}
		cmd = cmdpos;
		free(pid);
		free(vmem);
		free(state);
		free(cmd);
		free(buffer);
		closedir(procdir);


		sleep(t);
		system("clear");

	}


}