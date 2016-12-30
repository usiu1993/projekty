#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
 #include <errno.h>
#include <signal.h>
#include <sys/time.h>
 
 
#define MAX 10000

void timer_handler (int signum)
{
	//struct timespec time2;
 static int count = 0;
 printf ("timer expired %d times\n", ++count);
 
 

}


void display_message(int s) {
	struct timespec time2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
 
 printf("!!!!!  Stil alive :sec %ld, nsec %ld\n",time2.tv_sec,time2.tv_nsec);
     alarm(0);    //for every second
     signal(SIGALRM, display_message);
}



int main(int argc, char** argv)
{
	

	struct timespec time1;
	int opt;
	char* str2;
	int ret_value;
	float d;
	
struct sigaction sa;
 struct itimerval timer;


	
	
	
	
	
 while ((opt = getopt(argc, argv, ":c:")) != -1) 
	{
               switch (opt) 
	       {
              
               case 'c':
                  str2 = optarg;
                   break;
              /* case 'd':
					d=atof(optarg);
					break;
					*/
               default:
                   printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                   exit(1);
               }
    }
       

//----------------------------------------------------------------------


     /* Install timer_handler as the signal handler for SIGVTALRM. */
 memset (&sa, 0, sizeof (sa));
 sa.sa_handler = &timer_handler;
 sigaction (SIGVTALRM, &sa, NULL);

 /* Configure the timer to expire after 250 msec... */
 timer.it_value.tv_sec = 2;
 timer.it_value.tv_usec = 0;
 /* ... and every 250 msec after that. */
 timer.it_interval.tv_sec = 2;
 timer.it_interval.tv_usec = 0;
 /* Start a virtual timer. It counts down whenever this process is
   executing. */
 setitimer (ITIMER_VIRTUAL, &timer, NULL);

    //------------------------------------------------
		
	char *myfifo2 = str2;
	
char str4[BUFSIZ];
	int server_to_client;
	//O_RDONLY
	
char str1[31];
long int time11, time2;

//while(1);
signal(SIGALRM, display_message);
    alarm(0);
    
    if(access(myfifo2,F_OK) != -1)
		server_to_client = open(myfifo2,O_RDONLY);
while(read(server_to_client,str1,sizeof(str1)))
{

  
  // perror("Read:"); // Very crude error check

printf("...received: %s\n",str1);
//printf("HELLO\n");
sscanf(str1, "sec:%ld, nsec:%ld\n", &time11,&time2);
clock_gettime(CLOCK_REALTIME, &time1);
printf("..my time is   :sec %ld, nsec %ld\n",time1.tv_sec,time1.tv_nsec);
printf("...difference :sec %ld, nsec %ld\n",time1.tv_sec-time11,time1.tv_nsec-time2);
	
  memset(str1, 0, sizeof(str1));
}
   //nanosleep(&spi,NULL);
   // memset(str1, 0, sizeof(str1));
   //close(server_to_client[pid]);
   //int status;
		//waitpid(pid, &status, WUNTRACED | WCONTINUED);
//       ret_value = close(server_to_client);
        if (ret_value != 0)  {
            printf("\nFclose failed");
            printf("\nerrno is %d",errno);
            exit(8);
        }	

	//}
	
   return 0;
}
