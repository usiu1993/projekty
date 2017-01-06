#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#define NAME 25
#define BLOCK 31
int czas;

void komunikat(int sig) {
	struct timespec czasProc;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &czasProc);
 
 printf("!!!!!  Stil alive :sec %ld, nsec %ld\n",czasProc.tv_sec,czasProc.tv_nsec);
     alarm(czas);    //for every second
     signal(SIGALRM, komunikat);
}



int main(int argc, char** argv)
{
	struct timespec czasAlarm;
	int opt;
	//char nazwaFifo[50];
	char* nazwaFifo = malloc(NAME*sizeof(char*));
	int ret_value;
	int fd;	
	char otrzymDane[BLOCK+2];
	//char* otrzymDane=malloc((BLOCK+2)*sizeof(char*));
	long int czasPorSek, czasPorNsek;
	//float d;
	
//struct sigaction sa;
 //struct itimerval timer;


	
	
	
	
	
 while ((opt = getopt(argc, argv, "d:")) != -1) 
	{
               switch (opt) 
	       {
              
               case 'd':
					czas=atof(optarg);
					break;
					
               default:
                   printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                   exit(1);
               }
    }
    if(optind<argc)
    {
		while(optind<argc)
			{
					strncpy(nazwaFifo,argv[optind],NAME);
					printf("%s \n",nazwaFifo);
					optind++;
				}
		
		}
       


signal(SIGALRM, komunikat);
    alarm(czas);
    
    if(access(nazwaFifo,F_OK) != -1)
		fd = open(nazwaFifo,O_RDONLY);
while(read(fd,otrzymDane,sizeof(otrzymDane)))
{
fflush(stdout);
printf("----------------------------\n");
printf("...otrzymano:       %s\n",otrzymDane);
sscanf(otrzymDane, "sec:%ld, nsec:%ld", &czasPorSek,&czasPorNsek);
clock_gettime(CLOCK_REALTIME, &czasAlarm);
printf("...czas odbiornika: sec %ld, nsec %ld\n\n",czasAlarm.tv_sec,czasAlarm.tv_nsec);
long int sec=czasAlarm.tv_sec-czasPorSek;
long int nsec=czasAlarm.tv_nsec-czasPorNsek;
printf("...różnica:         sec %ld, nsec %ld\n",sec,nsec);
	
  //memset(otrzymDane, 0, sizeof(otrzymDane));
}
	ret_value=close(fd);
        if (ret_value != 0)  {
            printf("\nFclose failed");
            printf("\nerrno is %d",errno);
            exit(EXIT_FAILURE);
        }	

	free(nazwaFifo);

	
   return 0;
}
