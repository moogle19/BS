#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

double count(int max)
{
  int i = 0, j = 0;
  time_t start, end;
  double diff;
  
  time(&start);
  for(i = 0; i < max; i++)
  {   
    j++;
  }
  time(&end);
  diff = difftime(end, start);
  
  return diff;
}

int main(void)
{
  int i = 0;
  int status = 0;
  printf("parent\n");
  for(i = 0; i < 10; i++)
  {
    pid_t pid = fork();
    
    if(pid == 0)
    {
      printf("child");
      printf("%s%f\n", "Time: ", count(100000000)); 
      exit(0);
    }
    wait(&status);
  }
}
