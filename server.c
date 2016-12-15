#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	int opt;
//    int desc;
	char buffer[255]="teskt do wyslania";
	//char readb[255];
//	int pid;
	int fd[2];
	pipe(fd);
	close(fd[0]);
	if(argc!=2)
	{
		printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                exit(1);

	}
	
	
	while ((opt = getopt(argc, argv, "d:")) != -1) 
	{
               switch (opt) 
	       {
               case 'd':
				printf("opt to %s\n",optarg);
                   fd[1] = atoi(optarg);
                   printf("CHUJ to %d\n",fd[1]);
                   break;
               default:
                   printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                   exit(1);
               }
        }
//int fd[2]={0};
	//pipe(desc);

//printf("czas to %d",desc);
//close(fd[0]);
		struct timespec spi;
		spi.tv_sec=3;
		spi.tv_nsec=0;
		for(;;)
		{
			
			write(fd[1],buffer,strlen(buffer)+1);
			nanosleep(&spi,NULL);
		}
		
		return 0;
		
}
