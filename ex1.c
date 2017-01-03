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
int server_to_client;
char* str2;
char* str3;
int ret_value;
float d=0;

int PIPE=0;

//----------------------------------------------------------------------------


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

void timer_handler (int signum)
{
 static int count = 0;
 struct timeval ts;
 count += 1;
 gettimeofday(&ts, NULL);	
 printf ("  timer expired %d times\n",  count);

}
void signal_callback_handler(int signum){

     //   printf("Caught signal SIGPIPE %d\n",signum);
}



int main(int argc, char *argv[])
{   

 struct sigaction sa1;
 struct itimerval timer;
 struct timespec time1;

 int num;
 int opt;
 int fd[2];
	pipe(fd);
    close(fd[0]);


//clockid_t flag;
    struct sigevent sev;
    struct itimerspec its;
struct itimerspec it;
    //long long freq_nanosecs;
   // sigset_t mask;
    struct sigaction sa;
    clockid_t flag;

        while ((opt = getopt(argc, argv, ":m::d::w::c::p:f::s::")) != -1) 
	{
               switch (opt) 
	       {
	       case 'm':
		 num=atof(optarg);
                   break;
               case 'd':
    		d=atof(optarg);
                   break;

               case 'w':
               // printf("Establishing handler for signal %d\n", SIG);
   		
		flag=CLOCK_REALTIME;
		//flag=CLOCK_PROCESS_CPUTIME_ID;

                   break;
               case 'c':
    		//printf("Establishing handler for signal %d\n", SIG);
   	
		flag = CLOCK_MONOTONIC;
		//flag =CLOCK_PROCESS_CPUTIME_ID;
                   break;
               case 'p':
		    //printf("Establishing handler for signal %d\n", SIG);
   	
		flag =CLOCK_PROCESS_CPUTIME_ID;
                   break;
		case 'f':
			str2 = optarg;
			break;
		case 's':
			pipe(fd);
			fd[1]=atoi(optarg);
			PIPE=1;
			break;
			
               default:
                   printf("Flaga to Cos poszlo nietak\n");
                   exit(1);
               }
         
        }


		sa.sa_flags = SA_SIGINFO;
   		sa.sa_sigaction = handler;
   		 sigemptyset(&sa.sa_mask);
    		sigaction(SIG, &sa, NULL);

		    sev.sigev_notify = SIGEV_SIGNAL;
		    sev.sigev_signo = SIG;
		    sev.sigev_value.sival_ptr = &timerid;
		    timer_create(flag, &sev, &timerid);


	
/* Install timer_handler as the signal handler for SIGVTALRM. */
 memset (&sa1, 0, sizeof (sa1));
 sa1.sa_handler = &timer_handler;
 sigaction (SIGVTALRM, &sa1, NULL);

 /* Configure the timer to expire after 500 msec... */
 timer.it_value.tv_sec = 8;
 timer.it_value.tv_usec = 0;
 /* ... and every 500 msec after that. */
 timer.it_interval.tv_sec = 8;
 timer.it_interval.tv_usec = 0;
 /* Start a virtual timer. It counts down whenever this process is
   executing. */
//--------------------------------------------------------------
  char *buf=(char*) malloc(50*sizeof(*buf));	
  char *myfifo2=(char*) malloc(50*sizeof(*myfifo2));
  myfifo2=str2;
  
  
    if (access(myfifo2, F_OK) == -1){
			mkfifo(myfifo2, 0666);
		}
	else 
		printf("utworzono wczesniej \n");
	
   server_to_client = open(myfifo2, O_WRONLY);
  
// printf("Server ON.\n");

 //printf("Sending...\n");
//-------------------------------------------------------------
	//signal(SIGPIPE, signal_callback_handler);
	
    its.it_value.tv_sec =10;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    timer_settime(timerid, 0, &its, NULL);
float rtime;
   
	 while (g_running){
		rtime = (num-d)+(float)rand() / RAND_MAX * (2*d);
	//printf("rtime %lf",rtime);
	while(rtime<=0)
		rtime = (num-d)+(float)rand() / RAND_MAX * (2*d);
		
	sleep(rtime);
	timer_gettime (timerid, &it);
	//printf("GET : sec:%ld, nsec:%ld\n",it.it_value.tv_sec,it.it_value.tv_nsec);
	clock_gettime(CLOCK_REALTIME, &time1);
	//for (int i = 0; i< 242000000; i++)
	//	temp+=temp;
	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	printf("sec:%ld, nsec:%ld\n",time1.tv_sec,time1.tv_nsec);
	fflush(stdout);
//----------------------------------------------------------------
	snprintf(buf, 31, "sec:%ld, nsec:%ld\n",time1.tv_sec,time1.tv_nsec);
	//write(STDOUT_FILENO, buf, sizeof(buf)-1);
	/*if(!PIPE){
	write(server_to_client, buf, strlen(buf)+1);
	}
	else{
	write(fd[1], buf, strlen(buf)+1);	
	}*/
	
	//printf("buf to %s\n",buf);
//----------------------------------------------------------------
	 setitimer (ITIMER_VIRTUAL, &timer, NULL);


}

 close(server_to_client);
   
   
  ret_value = remove(myfifo2);
        if (ret_value != 0)  {
            printf("\nremove failed for %s",myfifo2);
            printf("\nerrno is %d",errno);
            exit(9);
        }
	unlink(myfifo2);
	free(myfifo2);
    //sleep(100);
    exit(EXIT_SUCCESS);
}
