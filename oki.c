#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1

timer_t timerid;
sig_atomic_t volatile g_running = 1;
int server_to_client[6];
char** str2;
char* str3;
int ret_value;
float d=0;
int num;
int f;
int LICZ=0;
int PIPE=0;
int pipeIn;
char *buf;
struct itimerspec it;
struct timespec time1;
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
  sigaction (SIGVTALRM, &sa1, NULL);
  
  
	float timeA = (num-d)+(float)rand() / RAND_MAX * (2*d);

	while(timeA<=0)
		timeA = (num-d)+(float)rand() / RAND_MAX * (2*d);
	int cze2=(int)timeA;
	timeA=timeA-cze2;
	int cze3=timeA*1000000;
  
  
  timer1.it_value.tv_sec = cze2;
  timer1.it_value.tv_usec = cze3;
  
  timer_gettime (timerid, &it);
	
	clock_gettime(CLOCK_REALTIME, &time1);
  
	snprintf(buf, 32, "sec:%ld, nsec:%ld\n",time1.tv_sec,time1.tv_nsec);
	setitimer (ITIMER_VIRTUAL, &timer1, NULL);
	if(!PIPE){
		
	for(int i =1;i<=f;i++)
	{

	write(server_to_client[i],buf,32);
	}


	}
	else{

	write(pipeIn,buf,32);	
		
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
g_running = 0;
    
    exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{   


str2 = malloc(50*sizeof(char*));	
 int m=0;
 int req=1;
  int F=0;
 int opt;
 float czas;
 int fd[2];
	pipe(fd);
    close(fd[0]);
char **gg = malloc(1*sizeof(char*));
    struct sigevent sev;
    struct itimerspec its;
	srand( time( NULL ) );

    struct sigaction sa;
    clockid_t flag;
	
	f=0;
	
	if(argc<3)
	{
				printf(" ARG Required -m[sec] -f(fifo)/s(pipe)\n");
                exit(1);

	}


        while ((opt = getopt(argc, argv, "m:d:w:c:p:f:s:")) != -1) 
	{
               switch (opt) 
	       {
	       case 'm':
	     m++;
		 num=atof(optarg);
                   break;
               case 'd':
    		d=atof(optarg);
                   break;

               case 'w':
               req--;
   		czas=atof(optarg);
		flag=CLOCK_REALTIME;
		break;
               case 'c':
        if(req!=0){
    	req--;
		czas=atof(optarg);	
		flag = CLOCK_MONOTONIC;
		break;
		}
		else{		
		printf("option already chosen\n");
		exit(2);
		}
			
             case 'p':
		if(req!=0){
		req--;
		czas=atof(optarg);
		flag =CLOCK_PROCESS_CPUTIME_ID;
		break;
		}
		else{		
		printf("option already chosen\n");
		exit(2);
		}
           
		case 'f':
			f++;
			F++;
			gg[f] = malloc(15*sizeof(char));
			str2[f] = malloc(15*sizeof(char));
			gg=realloc(gg,15*f*sizeof(char*));
			str2[f] = optarg;
			break;
		case 's':
			printf("jestem tu\n");
			F++;
			pipeIn=atoi(optarg);
			PIPE=1;
			break;
			
       
		
               default:
                   printf("Wymagane -m[sec] -f(fifo)/s(pipe)\n");
                   exit(1);
               }
         
         
        }
        
 //------------------------------------------------------------

	    if(!F||!m){
			printf("nie wybrano m oraz f lub s\n");
			exit(1);
		}
		
	//-----------------------------------------------	
		

		sa.sa_flags = SA_SIGINFO;
   		sa.sa_sigaction = handler;
   		 sigemptyset(&sa.sa_mask);
    		sigaction(SIG, &sa, NULL);

		    sev.sigev_notify = SIGEV_SIGNAL;
		    sev.sigev_signo = SIG;
		    sev.sigev_value.sival_ptr = &timerid;
		    timer_create(flag, &sev, &timerid);


	

//--------------------------------------------------------------
  buf=(char*) malloc(50*sizeof(*buf));
  char **myfifo2 = malloc(f*sizeof(char*));	
  for(int i=1; i<=f;i++)
	{
		myfifo2[i] = malloc(15*sizeof(char));
		myfifo2[i]=str2[i];
		if (access(myfifo2[i], F_OK) == -1){
			mkfifo(myfifo2[i], 0666);
		}
		server_to_client[i] = open(myfifo2[i], O_WRONLY | O_NONBLOCK);
	}
  
	int cz=(int)czas;
	czas=czas-cz;
	int cz1= czas*1000000000;
    its.it_value.tv_sec =cz;
    its.it_value.tv_nsec = cz1;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    timer_settime(timerid, 0, &its, NULL);
  	
  struct itimerval timer;
	
  /* Install timer_handler as the signal handler for SIGVTALRM.  */
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &timer_handler;
  sigaction (SIGVTALRM, &sa, NULL);
	float czas2=(num-d)+(float)rand() / RAND_MAX * (2*d);
	while(czas2<=0)
		czas2 = (num-d)+(float)rand() / RAND_MAX * (2*d);
	int cz2=(int)czas2;
	czas2=czas2-cz2;
	int cz3=czas2*1000000;
	
	
  /* Configure the timer to expire after 250 msec...  */
  timer.it_value.tv_sec = cz2;
  //printf("timer sec: %ld\n",timer.it_value.tv_sec);
  timer.it_value.tv_usec = cz3;
  /* ... and every 250 msec after that.  */
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 0;
  /* Start a virtual timer.  It counts down whenever this process is
     executing.  */
  setitimer (ITIMER_VIRTUAL, &timer, NULL);
		
   
	 while (g_running){
		
		}

for(int i=1;i<=f;i++)
{
	close(server_to_client[i]);
	ret_value = remove(myfifo2[i]);
        if (ret_value != 0)  {
            printf("\nremove failed for %s",myfifo2[i]);
            printf("\nerrno is %d",errno);
            exit(9);
        }
	unlink(myfifo2[i]);
	free(myfifo2[i]);
    exit(EXIT_SUCCESS);
  
}
}
