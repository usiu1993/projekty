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


void copy_stdin2stdout(int* where,int size)
{
	 ssize_t bytes;
   size_t total_bytes;
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
			//	 FD_ZERO(&fdset);
            //FD_SET(where[i],&fdset);
				//ret = select(where[i]+1,&fdset,NULL,NULL,NULL);
				//printf("ret %d\n",ret);
				total_bytes = 0;
				
				  if ((bytes = write(where[i], buffer, 31)) == -1){
            
            printf("elooo\n");
            sleep(5);
            //perror("write");
		}
        else
            printf("speak: wrote %ld bytes\n", bytes);
				
				
				
				//bytes=write(where[i], buffer, 31);
			/*	if (write(where[i], buffer, 31) < 0) {
					printf("Failed to write into fifo: %s", strerror(errno));
					where[i]=0;
					exit(1); */
				/*if (bytes > 0) {
                    total_bytes += (size_t)bytes;
                    printf("done reading (%lu bytes)\n", total_bytes);
               }
               else{
                    if (errno == EWOULDBLOCK) {
                        /* Done reading 
                        printf("ararar");
                        break;
                    } else {
                        perror("read");
                      //  return EXIT_FAILURE;
                    }	
				}*/
}
				//bytes += write(where[i], buffer, 31);
				//printf("Pipsiz: %d\n",(int)bytes);
				
				}
			}
			 break;
		//write(where[2], buffer, strlen(buffer)+1);
}
       //write(w2, buffer, strlen(buffer)+1);
        //fflush(stdout);
    //    if (bytes < BLOCK_SIZE)
      //      if (feof(stdin))
      //      {  //memset(buffer, 0, sizeof(buffer));
               
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
	struct stat statbuf;
	char* nr=(char*) malloc(2*sizeof(*nr));



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
	
	char *wzorzec=(char*) malloc(15*sizeof(*nr));
	int *fd=(int*) malloc(2*sizeof(*fd));
	strcpy(wzorzec, nazwa);

	//-------------------------------------------------
	char **a = malloc(sizeof *a * c);
if (a)
{
  for (int i = 1 ; i<= c; i++)
  {
    a[i] = malloc(sizeof *a[i] * 30);
    sprintf(nr,"%d",i);
	strcat(wzorzec, nr);
	if (stat(wzorzec, &statbuf) == -1) { perror("stat"); continue; }	
	
	
	if (S_ISFIFO(statbuf.st_mode))
		strcpy(a[i],wzorzec);
	else 
		a[i]=NULL;
    
    
    strcpy(wzorzec, nazwa);
  }
}
	int *d=(int*) malloc(10*sizeof(*d));
	for(int j=1;j<c;j++)
	{
		
		printf("%s\n",a[j]);
		printf("------------\n");
		}
	
	
	
	
	/*while(a[k]!=NULL)
	{
		fd[k]=open(a[k],O_WRONLY);	
		printf("fd[%d] zwrocilo %d\n",k,fd[k]);
		if(fd[k]!=0)
			d[k]=fd[k];	
		k++;
	}*/
	//int pipesiz=100;
	for(int k=1;k<=c;k++)
	{
		fd[k]=open(a[k],O_WRONLY | O_NONBLOCK);	
		printf("fd[%d] zwrocilo %d\n",k,fd[k]);
		//fcntl(fd[k],F_SETPIPE_SZ,100);
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
	
	
	//printf("%s",wzorzec);
	//for(int i=1;i<=c;i++)
//{

//sprintf(nr,"%d",i);
//strcat(wzorzec, nr);
//printf("nazwa: %s\n",wzorzec);
//if (stat(wzorzec, &statbuf) == -1) { perror("stat"); continue; }	
//if (S_ISFIFO(statbuf.st_mode))
//{ 
//printf("File is a fifo pipe\n");
//a[i]=wzorzec;
//printf("w tablicy %s a i to %d\n",a[i],i);

/*

a[1]="/tmp/1";
a[2]="/tmp/2";


int fd = open(a[1],O_WRONLY);
int fd1=open(a[2],O_WRONLY);
printf("Fd dla  zworiclo %d\n",fd);
printf("Fd1 dla  zworiclo %d\n",fd1);
if(fd!=0)
{
	//int server_to_client;
   //server_to_client = open(, O_WRONLY);
	copy_stdin2stdout(fd,fd1);
}

else
printf("File is not a fifo\n");

//strcpy(wzorzec, nazwa);
//printf("nazwa: %s\n",nazwa);
//}
//printf("w tablicy %s\n",a[1]);



*/
	
/*	
 char *myfifo2=(char*) malloc(15*sizeof(*myfifo2));
 myfifo2=nazwa;
  
  
   if (access(myfifo2, F_OK) == -1){
			mkfifo(myfifo2, 0666);
		}
	else 
		printf("utworzono\n");
	
	int server_to_client;
   server_to_client = open(myfifo2, O_WRONLY);
  
 printf("Server ON.\n");

 printf("Sending...\n");




copy_stdin2stdout(server_to_client);
printf("%d\n",c);
return 0;
* */
}
