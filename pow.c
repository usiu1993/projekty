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
#define BLOCK_SIZE 512


void copy_stdin2stdout(int where)
{
    char buffer[BLOCK_SIZE];
    for(;;) {
        size_t bytes = fread(buffer,  sizeof(char),BLOCK_SIZE,stdin);
       // fwrite(buffer, sizeof(char), bytes, stdout);
       write(where, buffer, strlen(buffer)+1);
        //fflush(stdout);
        if (bytes < BLOCK_SIZE)
            if (feof(stdin))
            {  memset(buffer, 0, sizeof(buffer));
                break;
    }}

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


char **a = malloc(sizeof *a * 6);
if (a)
{
  for (int i = 0; i < 6; i++)
  {
    a[i] = malloc(sizeof *a[i] * 15);
  }
}
	  while ((opt = getopt(argc, argv, ":p:c::L::")) != -1) 
	{
               switch (opt) 
	       {
	       case 'p':
		 nazwa=optarg;
		 printf("nazwa to %s \n",nazwa);
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
	strcpy(wzorzec, nazwa);
	//printf("%s",wzorzec);
	for(int i=1;i<=c;i++)
{

sprintf(nr,"%d",i);
strcat(wzorzec, nr);
printf("nazwa: %s\n",wzorzec);
if (stat(wzorzec, &statbuf) == -1) { perror("stat"); continue; }	
if (S_ISFIFO(statbuf.st_mode))
{ 
printf("File is a fifo pipe\n");
a[i]=wzorzec;
printf("w tablicy %s a i to %d\n",a[i],i);
int fd = open(a[i],O_WRONLY);
printf("Fd dla  zworiclo %d\n",fd);
if(fd!=0)
{
	//int server_to_client;
   //server_to_client = open(, O_WRONLY);
	copy_stdin2stdout(fd);
	}
}
else
printf("File is not a fifo\n");

strcpy(wzorzec, nazwa);
//printf("nazwa: %s\n",nazwa);
}
printf("w tablicy %s\n",a[1]);




	
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
