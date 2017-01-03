#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define BLOCK_SIZE 31


	


void signal_callback_handler(int signum){

        printf("Caught signal SIGPIPE %d\n",signum);
        
}




void copy_stdin2stdout(int* where,int size)
{
	struct sigaction mySignalStruct;
	mySignalStruct.sa_handler = signal_callback_handler;
    mySignalStruct.sa_flags   = 0;    

    sigemptyset( &mySignalStruct.sa_mask); 
    sigaddset  ( &mySignalStruct.sa_mask, SIGINT);

    if (sigaction(SIGPIPE, &mySignalStruct, NULL) < 0)
    {
        fprintf(stderr, "sigaction error\n");
        exit(1);
    }
	 ssize_t bytes;
   //size_t total_bytes;
   //fd_set fdset;
   //int ret;
    char buffer[BLOCK_SIZE];
    for(;;) {
		//size_t size1=50;
		
		//----------------------------------------------
		while(read(STDIN_FILENO, buffer, 31)){
	
       for(int i=1;i<size;i++){
		   
		//printf("where[%d] to %d\n",i,where[i]);
			if(where[i]!=0){
				signal(SIGPIPE, signal_callback_handler);	
				if ((bytes = write(where[i], buffer, BLOCK_SIZE)) == -1){
                       where[i]=0;
                       printf("wyrzucam kanał /tmp/%d\n",i);   
					}
				else if(bytes<BLOCK_SIZE)
				{
					where[i]=0;
					printf("wyrzucam kanał /tmp/%d\n",i); 
				//KANAŁ PRZEPEŁNIONY DO WYJEBANIA 
				}
				else
				printf("wrote %ld bytes\n", bytes);
				
				}
			}}
			 break;
		
}
     
              
    }




int file_exist (char *filename)
{
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}






int main(int argc, char** argv)
{
	int opt;
	int c;
	char* nazwa;
	//struct stat statbuf;
	char* nr=(char*) malloc(5*sizeof(*nr));



	  while ((opt = getopt(argc, argv, ":p:c::L::")) != -1) 
	{
               switch (opt) 
	       {
	       case 'p':
		 nazwa=optarg;
		 //printf("nazwa to %s \n",nazwa);
                   break;
           case 'c':
    		c=atoi(optarg);
                   break;
           case 'L':
			
				break;
		default:
                   printf("Cos poszlo nietak\n");
                   exit(1);
		
			}
	}
	
	char *wzorzec=(char*) malloc(50*sizeof(*nr));
	int *fd=(int*) malloc(10*sizeof(*fd));
	strcpy(wzorzec, nazwa);

	//-------------------------------------------------
	char **a = malloc(sizeof *a * c);
//if (a)
//{
  for (int i = 1 ; i<c; i++)
  {struct stat statbuf;
    a[i] = malloc(sizeof *a[i] * 100);
    sprintf(nr,"%d",i);
	strcat(wzorzec, nr);
	//if (
	//stat(wzorzec, &statbuf) == -1) 
	//{ perror("stat"); continue; }	
	//printf("wzorzec to %s\n",wzorzec);
		memset (&statbuf, 0, sizeof (statbuf));
	stat(wzorzec, &statbuf); 
	//int t=S_ISFIFO(statbuf.st_mode);
	if (S_ISFIFO(statbuf.st_mode))
	{	printf("w fifo %d\n",i);
		strcpy(a[i],wzorzec);
	}
	 
		
    
    
    strcpy(wzorzec, nazwa);
    //printf("wzorzec na koncu %s\n",wzorzec);
  }
//}
	int *d=(int*) malloc(10*sizeof(*d));
	for(int j=1;j<c;j++)
	{
		
		printf("%s\n",a[j]);

		printf("------------\n");
		}
	
	
	
	
	for(int k=1;k<=c;k++)
	{
		fd[k]=open(a[k],O_WRONLY | O_NONBLOCK);	
		printf("fd[%d] zwrocilo %d\n",k,fd[k]);
		
		int pipesize = fcntl(fd[k], F_GETPIPE_SZ);
		printf("Pipe size: %d\n", pipesize);
		if(fd[k]!=-1)
			d[k]=fd[k];	
		}
	
		for(int j=1;j<=c;j++)
		{
		
		printf("w d %d\n",d[j]);
		printf("------------\n");
		}
	
	
	copy_stdin2stdout(d,c);
	
	return 0;

}
