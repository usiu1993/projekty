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
#define NAME 55
#define BLOCK 31
int czas;

//Wyświetlenie komunikatu,że odbiorca działa
void komunikat(int sig) {
	struct timespec czasProc;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &czasProc);

	printf("\n\nKomunikat odbiorcy  sec: %ld, nsec: %ld\n\n\n",czasProc.tv_sec,czasProc.tv_nsec);
	alarm(czas);    
	signal(SIGALRM, komunikat);
}



int main(int argc, char** argv)
{
	struct timespec czasAlarm;
	int opt;
	char* nazwaFifo = malloc(NAME*sizeof(char*));
	int ret_value;
	int fd;	
	char otrzymDane[BLOCK+2];
	long int czasPorSek, czasPorNsek;


	while ((opt = getopt(argc, argv, "d:")) != -1) 
	{
		switch (opt) 
		{

			case 'd':
				czas=strtod(optarg,NULL);
				break;

			default:
				printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
				exit(1);
		}
	}
	//Odczytanie ścieżki fifo bez parametru
	if(optind<argc)
	{
		while(optind<argc)
		{
			strncpy(nazwaFifo,argv[optind],NAME);
			printf("%s \n",nazwaFifo);
			int a =mkfifo(nazwaFifo, 0666);
		printf("otworzyl %s ale a= %d\n",nazwaFifo,a);
			optind++;
		}

	}


	//Komunikat o istnieniu odbiornika
	signal(SIGALRM, komunikat);
	alarm(czas);

	if(access(nazwaFifo,F_OK) != -1){
	//	mkfifo(nazwaFifo, 0666);
		//printf("otworzyl\n");
		fd = open(nazwaFifo,O_RDWR);
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
		printf("\nFclose failed");
		printf("\nerrno is %d",errno);
		exit(EXIT_FAILURE);
	}	

	free(nazwaFifo);


	return 0;
}
