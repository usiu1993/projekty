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
#define NAME 15
#define BLOCK 31
#define errorExit(blad) { fprintf(stderr,blad); fprintf(stderr, "nr bledu: %d\n", errno); exit(EXIT_FAILURE); } 


timer_t timerid;
sig_atomic_t volatile active = 1;
int server_to_client[6];
char** kanalFifo;
//char* str3;
int ret;
float odchyl=0;
int odstep;
int liczFifo;
//int LICZ=0;
int PIPE=0;
int pipeWe;
char *wiad;
struct itimerspec itimer;
struct timespec czasReal;
int errno;
int errnum;


//----------------------------------------------------------------------------

void timer_handler (int signum)
{
	//static int count = 0;
	//printf ("timer expired %d times\n", ++count);
	struct sigaction sa1;
	struct itimerval timer1;
	srand( time( NULL ) );
	/* Install timer_handler as the signal handler for SIGVTALRM.  */
	memset (&timer1, 0, sizeof (timer1));
	memset (&sa1, 0, sizeof (sa1));
	sa1.sa_handler = &timer_handler;
	//sigaction (SIGVTALRM, &sa1, NULL);
	if (sigaction (SIGVTALRM, &sa1, NULL) == -1)
	{			
		errnum = errno;
		fprintf(stderr,"sigaction error");
		fprintf(stderr, "Nr bledu: %d\n", errno); 
		exit(EXIT_FAILURE);
	}
	//  errExit("sigaction");


	float timeA = (odstep-odchyl)+(float)rand() / RAND_MAX * (2*odchyl);

	while(timeA<=0)
		timeA = (odstep-odchyl)+(float)rand() / RAND_MAX * (2*odchyl);
	int cze2=(int)timeA;
	timeA=timeA-cze2;
	int cze3=timeA*1000000;


	timer1.it_value.tv_sec = cze2;
	timer1.it_value.tv_usec = cze3;

	timer_gettime (timerid, &itimer);

	clock_gettime(CLOCK_REALTIME, &czasReal);

	snprintf(wiad, 32, "sec:%ld, nsec:%ld\n",czasReal.tv_sec,czasReal.tv_nsec);
	setitimer (ITIMER_VIRTUAL, &timer1, NULL);
	if(!PIPE){

		for(int i =1;i<=liczFifo;i++)
		{

			write(server_to_client[i],wiad,32);
		}


	}
	else{

		write(pipeWe,wiad,32);	

	}


}



static void handler(int sig, siginfo_t *si, void *uc)
{
	if(si->si_value.sival_ptr != &timerid){
		printf("Stray signal\n");
	} else {
		// printf("Caught signal %d from timer\n", sig);
	}
	//exit(1);
	active = 0;

	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{   


	//str2 = malloc(50*sizeof(char*));	
	int obowCzas=0;
	int wyborZeg=1;
	int wyborKan=0;
	int opt;
	float czas;
	//int fd[2];
	//pipe(fd);
	//close(fd[0]);
	kanalFifo = malloc(1*sizeof(char*));
	struct sigevent sev;
	struct itimerspec itimspec;
	srand( time( NULL ) );

	struct sigaction sa;
	clockid_t zegar;

	liczFifo=0;

	if(argc<3)
	{
		printf("Argumenty wymagane: -m[sec] -f(fifo)/s(pipe)\n");
		exit(1);

	}


	while ((opt = getopt(argc, argv, "m:d:w:c:p:f:s:")) != -1) 
	{
		switch (opt) 
		{
			case 'm':
				obowCzas++;
				odstep=atof(optarg);
				break;
			case 'd':
				odchyl=atof(optarg);
				break;

			case 'w':
				wyborZeg--;
				czas=atof(optarg);
				zegar=CLOCK_REALTIME;
				break;
			case 'c':
				if(wyborZeg!=0){
					wyborZeg--;
					czas=atof(optarg);	
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
					czas=atof(optarg);
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
				//	gg[f] = malloc(15*sizeof(char));
				kanalFifo[liczFifo] = malloc(NAME*sizeof(char));
				kanalFifo[liczFifo] = optarg;
				kanalFifo=realloc(kanalFifo,NAME*liczFifo*sizeof(char*));
				break;
			case 's':
				wyborKan++;
				pipeWe=atoi(optarg);
				PIPE=1;
				break;



			default:
				printf("Wymagane -m[sec] -f(fifo)/s(pipe)\n");
				exit(1);
		}


	}

	//------------------------------------------------------------

	if(!wyborKan||!obowCzas){
		printf("nie wybrano m oraz f lub s\n");
		exit(1);
	}

	//-----------------------------------------------	


	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	//sigaction(SIG, &sa, NULL);

	if (sigaction(SIG, &sa, NULL) == -1)
	{
		errorExit("sigaction error\n");
	}

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;
	//timer_create(zegar, &sev, &timerid);

	if (timer_create(zegar, &sev, &timerid) == -1)
	{
		errorExit("timer_create error\n");
		//errnum = errno;
		//fprintf(stderr,"timer_create error\n");
		//fprintf(stderr, "Nr bledu: %d\n", errno); 
		//exit(EXIT_FAILURE);
	}




	//--------------------------------------------------------------
	wiad=(char*) malloc(BLOCK*sizeof(*wiad));
	char **fifo = malloc(liczFifo*sizeof(char*));	
	for(int i=1; i<=liczFifo;i++)
	{
		fifo[i] = malloc(NAME*sizeof(char));
		fifo[i]=kanalFifo[i];
		if (access(fifo[i], F_OK) == -1){
			mkfifo(fifo[i], 0666);
		}
		server_to_client[i] = open(fifo[i], O_WRONLY | O_NONBLOCK);
		if(server_to_client[i]==-1)
		{
			//errorExit("open error\n");
			fprintf(stderr,"open error for %s\n",fifo[i]); 
			fprintf(stderr, "Nr bledu: %d\n", errno);
		}
	}

	int czasSek=(int)czas;
	czas=czas-czasSek;
	int czasNsek= czas*1000000000;
	itimspec.it_value.tv_sec =czasSek;
	itimspec.it_value.tv_nsec = czasNsek;
	itimspec.it_interval.tv_sec = itimspec.it_value.tv_sec;
	itimspec.it_interval.tv_nsec = itimspec.it_value.tv_nsec;
	timer_settime(timerid, 0, &itimspec, NULL);

	struct itimerval timer;

	/* Install timer_handler as the signal handler for SIGVTALRM.  */
	memset (&sa, 0, sizeof (sa));
	sa.sa_handler = &timer_handler;
	//sigaction (SIGVTALRM, &sa, NULL);

	if (sigaction (SIGVTALRM, &sa, NULL) == -1)
	{
		errorExit("sigaction error\n");
	}


	float czasLos=(odstep-odchyl)+(float)rand() / RAND_MAX * (2*odchyl);
	while(czasLos<=0)
		czasLos = (odstep-odchyl)+(float)rand() / RAND_MAX * (2*odchyl);
	int czasLosSek=(int)czasLos;
	czasLos=czasLos-czasLosSek;
	int czasLosNsek=czasLos*1000000;


	/* Configure the timer to expire after 250 msec...  */
	timer.it_value.tv_sec = czasLosSek;
	//printf("timer sec: %ld\n",timer.it_value.tv_sec);
	timer.it_value.tv_usec = czasLosNsek;
	/* ... and every 250 msec after that.  */
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	/* Start a virtual timer.  It counts down whenever this process is
	   executing.  */
	setitimer (ITIMER_VIRTUAL, &timer, NULL);

	if (setitimer (ITIMER_VIRTUAL, &timer, NULL) == -1)
	{			errorExit("settimer error\n");
	}	

	while (active){

	}

	for(int i=1;i<=liczFifo;i++)
	{
		close(server_to_client[i]);
		ret = remove(fifo[i]);
		if (ret != 0)  {
			fprintf(stderr,"\nnie usunieto %s",fifo[i]);
			fprintf(stderr,"\nerrno: %d",errno);
			exit(9);
		}
		unlink(fifo[i]);
		free(fifo[i]);
		free(kanalFifo[i]);
		exit(EXIT_SUCCESS);

	}
	free(fifo);
	free(kanalFifo);
	free(wiad);
}
