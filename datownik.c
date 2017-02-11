#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1
#define NAME 60
#define ILOSC 50
#define NR 3
#define BLOCK 31
#define DANE 32
#define errorExit(blad) { fprintf(stderr,blad); fprintf(stderr, " nr bledu: %d\n", errno); exit(EXIT_FAILURE); } 


timer_t timerid;
sig_atomic_t volatile aktywny = 1;
int kanal[ILOSC];
int ret;
float odchyl=0;
float odstep;
int liczFifo;
int liczPipe;
int PIPE=0;
int FIFO=0;
char wiad[BLOCK];
struct itimerspec itimer;
struct timespec czasReal;
struct timespec tim;
int kanalPipe[ILOSC];

//void timer_handler (int signum)
void komunikacja();
void zegarUst(clockid_t zeg,float czasZ);
void timerUst(float ods,float odch,struct itimerval t);

	//handler dla pomiaru czasu odstępów między stemplami
static void handler(int sig, siginfo_t *signal, void *handler)
{
	if(signal->si_value.sival_ptr != &timerid)
		printf("Blad czasu sygnalu\n");
	
	aktywny = 0;
	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{   
	int obowCzas=0;
	int wyborZeg=1;
	int wyborKan=0;
	int opt;
	float czas;
	char kanalFifo[ILOSC][NAME];
	
	
	struct sigaction sa;
	clockid_t zegar;
	srand( time( NULL ) );
	liczFifo=0;
	liczPipe=0;
	
	if(argc<3)
	{
		printf("Argumenty wymagane: -m[sec] -f(fifo)/s(pipe)\n");
		exit(1);

	}

		//Wybór parametrów oraz kontrola ich ilości
	while ((opt = getopt(argc, argv, "m:d:w:c:p:f:s:")) != -1) 
	{
		switch (opt) 
		{
			case 'm':
				obowCzas++;
				odstep=strtod(optarg,NULL);
				break;
			case 'd':
				odchyl=strtod(optarg,NULL);
				break;

			case 'w':
				wyborZeg--;
				czas=strtod(optarg,NULL);
				zegar=CLOCK_REALTIME;
				break;
			case 'c':
				if(wyborZeg!=0){
					wyborZeg--;
					czas=strtod(optarg,NULL);	
					zegar = CLOCK_MONOTONIC;
					break;
				}
				else{		
					printf("do wyboru co najwyzej 1 zegar\n");
					exit(2);
				}

			case 'p':
				if(wyborZeg!=0){
					wyborZeg--;
					czas=strtod(optarg,NULL);
					zegar =CLOCK_PROCESS_CPUTIME_ID;
					break;
				}
				else{		
					printf("do wyboru co najwyzej 1 zegar\n");
					exit(2);
				}

			case 'f':
				liczFifo++;
				wyborKan++;
				strcpy(kanalFifo[liczFifo],optarg);
				printf("fifo: %s\n",kanalFifo[liczFifo]);
				FIFO=1;
				break;
			case 's':
				liczPipe++;
				wyborKan++;
				kanalPipe[liczPipe]=strtol(optarg,NULL,0);
				PIPE=1;
				break;

			default:
				printf("Wymagane -m[sec] -f(fifo)/s(pipe)\n");
				exit(1);
		}


	}

	if(!wyborKan||!obowCzas){
		printf("nie wybrano m oraz f lub s\n");
		exit(1);
	}

		// Konieczość mierzenia czasu timerem
		// dla pomiarów zegarem lokalnym procesu
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIG, &sa, NULL) == -1)
		{perror("sigaction error\n");
			exit(-1);
		}
		//errorExit("sigaction error\n");

		//Stworzenie i otwarcie plików fifo
	for(int i=1; i<=liczFifo;i++)
	{
		if (access(kanalFifo[i], F_OK) == -1){
			mkfifo(kanalFifo[i], 0666);
		}
		kanal[i] = open(kanalFifo[i], O_WRONLY); //zle
		if(kanal[i]==-1)
		{
			perror("fifo open error\n");
			exit(-1);
		}
	}
	if(!wyborZeg)
	{
		zegarUst(zegar,czas);
	}
		// Konfiguracja sygnału dla timera
	struct itimerval timer;
	memset (&sa, 0, sizeof (sa));
	timerUst(odstep,odchyl,timer);
	
	if (sigaction (SIGVTALRM, &sa, NULL) == -1)
		{
			perror("sigaction error\n");
			exit(-1);
		}
		
		
		//Praca programu
	while (aktywny){
		komunikacja();
		}


		//Zwolnienie pamięci
	for(int i=1;i<=liczFifo;i++)
	{
		close(kanal[i]);
		ret = remove(kanalFifo[i]);
		if (ret != 0)  {
			{
			perror("remove fifo error\n");
			exit(-1);
			}
		}
		unlink(kanalFifo[i]);

	}

}


void komunikacja()
{

	srand( time( NULL ) );
		//Losowy czas z podanego przedziału
	float czasLos = (odstep-odchyl)+(float)rand() / RAND_MAX * (2*odchyl);
	while(czasLos<=0)
		czasLos = (odstep-odchyl)+(float)rand() / RAND_MAX * (2*odchyl);
		//Obliczenie sekund i nanosekund	
	int czasLosSek=(int)czasLos;
	czasLos=czasLos-czasLosSek;
	int czasLosNsek=czasLos*1000000;	
		//Pobrnie czasu z zegara realnego
	clock_gettime(CLOCK_REALTIME, &czasReal);
	snprintf(wiad, 32, "sec:%ld, nsec:%ld\n",czasReal.tv_sec,czasReal.tv_nsec);
	
	tim.tv_sec = czasLosSek;
	tim.tv_nsec = czasLosNsek;
  
			//Pisanie do podanych fifo i pipe
		for(int i =1;i<=liczFifo;i++)
		{

			write(kanal[i],wiad,DANE);
		}
		
		for(int i=1;i<=liczPipe;i++)
		{
			
		write(kanalPipe[i],wiad,DANE);	
		
		}

	 nanosleep(&tim, NULL);

}

void zegarUst(clockid_t zeg,float czasZ)
{
	
	struct sigevent sev;
	struct itimerspec itimspec;
		//Utworzenie i konfiguracja wybranego timera pomiaru czasu
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;

	if (timer_create(zeg, &sev, &timerid) == -1)
		{
			perror("timer create error\n");
			exit(-1);
		}
		
		//Obliczenie sek/nsek dla timera odliczającego czas
	int czasSek=(int)czasZ;
	czasZ=czasZ-czasSek;
	int czasNsek= czasZ*1000000000;
	itimspec.it_value.tv_sec =czasSek;
	itimspec.it_value.tv_nsec = czasNsek;
	itimspec.it_interval.tv_sec = itimspec.it_value.tv_sec;
	itimspec.it_interval.tv_nsec = itimspec.it_value.tv_nsec;
	timer_settime(timerid, 0, &itimspec, NULL);
	
	
	}
	
	void timerUst(float ods,float odch,struct itimerval t)
{
		//Losowy czas z podanego przedziału
	float czasLos=(ods-odch)+(float)rand() / RAND_MAX * (2*odch);
	while(czasLos<=0)
		czasLos = (ods-odch)+(float)rand() / RAND_MAX * (2*odch);
		//Obliczenie sekund i nanosekund
	int czasLosSek=(int)czasLos;
	
	czasLos=czasLos-czasLosSek;
	int czasLosNsek=czasLos*1000000;
	
		//Ustawienie czasu pierwszego wygaśnięcia timera
	t.it_value.tv_sec = czasLosSek;
	t.it_value.tv_usec = czasLosNsek;
		//Następne czasy są losowane w handlerze
	t.it_interval.tv_sec = 0;
	t.it_interval.tv_usec = 0;
		//Ustawienie przygotowanego timera
	setitimer (ITIMER_VIRTUAL, &t, NULL);
	if (setitimer (ITIMER_VIRTUAL, &t, NULL) == -1)
				{
			perror("timer set error\n");
			exit(-1);
		}
	
	
	}
