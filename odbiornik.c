#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#define NAZWA 80
#define BLOCK 31
#define CLOCKID CLOCK_REALTIME


//Wyświetlenie komunikatu,że odbiorca działa
void komunikat(int sig) {
	fflush(stdout);
	struct timespec czasProc;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &czasProc);

	printf("\n\nKomunikat odbiorcy  sec: %ld, nsec: %ld\n\n\n",czasProc.tv_sec,czasProc.tv_nsec);
	 signal(SIGALRM,komunikat);

}


int main(int argc, char** argv)
{
	struct timespec czasAlarm;
	struct itimerval tim;
	int opt;
	//char* nazwaFifo = malloc(NAZWA*sizeof(char*));
	char nazwaFifo[NAZWA];
	int ret_value;
	int fd;	
	char otrzymDane[BLOCK+2];
	long int czasPorSek, czasPorNsek;
	float czas;

	while ((opt = getopt(argc, argv, "d:")) != -1) 
	{
		switch (opt) 
		{
			case 'd':
				czas=strtod(optarg,NULL);
				break;
		}
	}
	//Odczytanie ścieżki fifo bez parametru
	if(optind<argc)
	{
		while(optind<argc)
		{
			strncpy(nazwaFifo,argv[optind],NAZWA);
			mkfifo(nazwaFifo, 0666);
			optind++;
		}

	}
	
	
  
	int czasSek=(int)czas;
	czas=czas-czasSek;
	int czasNsek=czas*1000000;
  
  //Ustawienie czasu komunikatu 
  tim.it_interval.tv_sec = czasSek;
  tim.it_interval.tv_usec =czasNsek ;
  tim.it_value.tv_sec = czasSek; 
  tim.it_value.tv_usec = czasNsek;
  setitimer(ITIMER_REAL, &tim,0);

// Ustawienie alarmu
  signal(SIGALRM,komunikat); 
  
	if(access(nazwaFifo,F_OK) != -1){
		fd = open(nazwaFifo,O_RDONLY);
	}
	

	//Czytanie dopóki są dostarczane informacje
	while(read(fd,otrzymDane,sizeof(otrzymDane)))
	{
		
		fflush(stdout);
		printf("----------------------------\n");
		printf("...otrzymano:       %s\n",otrzymDane);
		sscanf(otrzymDane, "sec:%ld, nsec:%ld", &czasPorSek,&czasPorNsek);
		clock_gettime(CLOCK_REALTIME, &czasAlarm);
		printf("...czas odbiornika: sec:%ld, nsec:%ld\n\n",czasAlarm.tv_sec,czasAlarm.tv_nsec);

		//Porównanie czasów
		long int sec=czasAlarm.tv_sec-czasPorSek;
		long int nsec=czasAlarm.tv_nsec-czasPorNsek;
		printf("...różnica:         sec: %ld, nsec: %ld\n",sec,nsec);
	}	
	//Zamknięcie deskryptora
	ret_value=close(fd);
	if (ret_value != 0)  {
		{
			perror("close error\n");
			exit(-1);
		}
	}	
	unlink(nazwaFifo);


	return 0;
}
