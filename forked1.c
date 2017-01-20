#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>






int Ppid;
 int fd[10][2];
char buffer1[255];
     
   
    long iterator=0;

void komunikat(int sig) {
	FILE *fileptr= fopen("plik.txt", "rb");
if(getpid()==Ppid){
	//while(1){
      for(int i=0; i<10; i++)
		{	fseek(fileptr, iterator-1, SEEK_SET);
			fread(buffer1, 1, 1, fileptr);  
			printf("buffer to %s\n",buffer1);
			
			write(fd[i][1], buffer1, 1);
			iterator++;
		}

	}
    
	alarm(3);    
	signal(SIGALRM, komunikat);
}



int main(int argc, char *argv[])
{
	int IMP=1;
	 
    char buffer12[255];
    long filelen;
//    int i;
	
  // char* wys="tekst";
	//int forker,forker1;
	 Ppid=getpid();
	printf("parent pid :%d\n",Ppid);
    // create pipe descriptors
    for(int i =0; i<10; i++)
		pipe(fd[i]);

	int pid;
	int i;
	char wys[255]="costam";


 signal(SIGALRM, komunikat);
	alarm(0);

if(IMP!=0)
{
	printf("JEST TU DO CHUJA\n");
for(i = 0; i<9; i++) {
    pid = fork();
    if(pid < 0) {
        printf("Error");
        exit(1);
    } else if (pid == 0) {
        printf("Child (%d): %d\n", i + 1, getpid());
        break; 
    } else  {
       //wait(NULL);
    }
}
IMP=0;
}
	
	while(1){
	if(getpid()!=Ppid)
	{
		
		int diff=getpid()-Ppid;
		//printf("tu dziecko o %d a roznica to %d\n",getpid(),diff);
	
	
	
        close(fd[diff][1]);
        char ot[255];
        // now read the data (will block)
        read(fd[diff][0], ot,1);
        printf("Child(%d) received value: %s\n", getpid(), ot);
        memset(ot,0,1);
	}
  }


 
 //sleep(12);
    return 0;
}
