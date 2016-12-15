#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char** argv)
{	
	char time[10]={0};
	int opt;
    int ntime;
	char readb[255];
	int pid;
	int childNum;
	

	
	
	
	while ((opt = getopt(argc, argv, "c:t:")) != -1) 
	{
               switch (opt) 
	       {
               case 'c':
                  childNum= atoi(optarg);
                  break;
               case 't':
                   ntime = atof(optarg);
                   break;
               default:
                   printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                   exit(1);
               }
        }
	
	int fd[10][2];
	for(int =0 ;i<n;i++)
	{
		pipe(fd[i]);
		}
	int pids[10];
		//Tworzenie pipa 
	 //int Fpid=getpid();	
	for (int i = 0; i < childNum; ++i) {
		pid = fork();
		//fd[i][2]={0};
		pipe(fd[i]);
			if (pid) {
				continue;}	
			else if (pid == 0) {
				break;
    } 		else {
				printf("fork error\n");
				exit(1);
				}
	} */  
	
	printf("ilosc dzieci %d\n",childNum);
	
	//jedno dziecko
	pid=fork();
	pipe(fd[1]);
	
       
	if(pid==-1)
	{
		printf("Error creating child process");
		exit(1);
	}
	else if(pid==0)
	{
	char passwrite[256];
	char s[256]="dziala lallala";
	printf("ma byc 1 a jest %d\n",getpid()-Fpid);
	snprintf(passwrite,sizeof(passwrite), "%d", fd[1][1]);
	//char *execArgs[] = { "./server",passwrite,NULL };
	//execvp("./server", execArgs);
	execl("./server","./server","-s",s,"-t",ntime,"-d",passwrite,(char*)0);
	}
	else
	{
		struct timespec spi;
		spi.tv_sec=ntime;
		spi.tv_nsec=0;
		int ppid=getpid();
		printf("klient zamyka %d\n",pid-ppid);
		close(fd[1][1]);
		for(int i=0 ;i<50;i++)
		{	
			int r = read(fd[1][0],&readb,255);
			printf("Przeczytano %s czyli %d bajtow!\n",readb,r);
						nanosleep(&spi,NULL);

		}
	}
	return 0;
}
