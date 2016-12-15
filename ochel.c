#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	int opt;
        int ntime;
	char buffer[255]={0};
	char readb[255];
	int pid;
	int fd[2]={0};
	pipe(fd);
	if(argc!=3)
	{
		printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                exit(1);

	}
        while ((opt = getopt(argc, argv, "c:t:")) != -1) 
	{
               switch (opt) 
	       {
               case 'c':
                   strcpy(buffer,optarg);
                   break;
               case 't':
		printf("opt to %s\n",optarg);
                   ntime = atoi(optarg);
		printf("czas to %d",ntime);
                   break;
               default:
                   printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                   exit(1);
               }
        }
	if((pid=fork())==-1)
	{
		printf("Error creating child process");
		exit(1);
	}
	else if(pid!=0)
	{
		close(fd[0]);
		struct timespec spi;
		spi.tv_sec=ntime;
		spi.tv_nsec=0;
		for(;;)
		{
			nanosleep(&spi,NULL);
			write(fd[1],buffer,strlen(buffer)+1);
		}
	}
	else
	{
		int r=0;
		close(fd[1]);
		for(;;)
		{	
			r=read(fd[0],readb,255);
			printf("Przeczytano %d bajtow!\n",r);
		}
	}
	return 0;
}
