#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#define MAX 10000

int main(int argc, char** argv)
{
	//int ppid;
	int pid;
	int ntime;
	int childNum;
	int opt;
	//int j= 1;
	//int io[2];
	//pipe(io);
	//int ik;
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
        struct timespec spi;
		spi.tv_sec=ntime;
		spi.tv_nsec=0;
    
	for (int i = 0; i < childNum; ++i) {
		pid = fork();
			if (pid) {
				continue;}	
			else if (pid == 0) {
				break;
    } 		else {
				printf("fork error\n");
				exit(1);
				}
	}   
      
      
      //pid=fork();  
	if(pid==-1)
	{
		printf("Error creating child process");
		exit(1);
	}
	else if(pid==0)
	{   
	//printf("%d  ",childNum);	
	//for(int j=1;j<=childNum;j++)
	//spid=getpid();
	char buffer[800];
	sprintf(buffer,"-d%d",getpid());
	//printf("bufer to %s \n",buffer);
	
	char *execArgs[] = { "./serverP", "-s\"adasto gej\"","-t\"1\"",buffer,NULL };
	execvp("./serverP", execArgs);
	
	//}
		//j++;
		
		
		
	}
	else
	{	
	char *myfifo2 = "/tmp/new";
    char str1[BUFSIZ];
	int server_to_client[MAX];
	//close(io[1]);
	//read(io[0],&spid,sizeof(pid_t));
	//ppid=getpid();
	//for(int a=pid;a>ppid;a--){
	//printf("fathers pid: %d, childs pid: %d\n",ppid,pid);
   //int aa=pid;
  // printf("PID TO %d\n\n",aa);
	server_to_client[pid] = open(myfifo2, O_RDONLY | O_CLOEXEC);
	read(server_to_client[pid],str1,sizeof(str1));
//   }

   perror("Read:"); // Very crude error check

   printf("...received from the server: %s\n",str1);
   nanosleep(&spi,NULL);
    memset(str1, 0, sizeof(str1));
   close(server_to_client[pid]);
   //int status;
		//waitpid(pid, &status, WUNTRACED | WCONTINUED);
	//}
	}
   return 0;
}
