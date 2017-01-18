#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int fd[10][2];
  // char* wys="tekst";
	//int forker,forker1;
int Ppid=getpid();
	printf("parent pid :%d\n",Ppid);
    // create pipe descriptors
    for(int i =0; i<7; i++)
		pipe(fd[i]);
	//int I=1;
	int pid;
	int i;
	char wys[255]="costam";

if(getpid()==Ppid){
      for(int i=0; i<7; i++)
			write(fd[i][1], wys, sizeof(wys));
        printf("Parent(%d) send value: %s\n", getpid(), wys);
	}


for(i = 0; i<6; i++) {
    pid = fork();
    if(pid < 0) {
        printf("Error");
        exit(1);
    } else if (pid == 0) {
        printf("Child (%d): %d\n", i + 1, getpid());
        //exit(0);
        break; 
    } else  {
       wait(NULL);
    }
}
//printf("tu dziecko o %d \n",getpid());
	
	
	if(getpid()!=Ppid)
	{
		
		int diff=getpid()-Ppid;
		printf("tu dziecko o %d a roznica to %d\n",getpid(),diff);
	
	
	
        close(fd[diff][1]);
        char ot[3];
        // now read the data (will block)
        read(fd[diff][0], ot,sizeof(ot) );
        printf("Child(%d) received value: %s\n", getpid(), ot);
  }
   /*
   for (int i = 0; i < 1; ++i) {
  
    
    forker = fork();
    printf("jestem tu %d\n",i);
   
    if (forker && I!=0) {
		p=getpid();
		int differ=p-Ppid;
		close(fd[differ][0]);
//        val = 100;
        printf("jestem tu\n");
  

        // close the write descriptor
        close(fd[differ][1]);

		I=0;
        continue;
    } else if (forker == 0 ) {
		
		
        break;
    } 
    else {
        printf("fork error\n");
        exit(1);
    }
}
   

forker1=getpid();


        // close the read-descriptor
        close(fd[diff][0]);
    
  
*/    
    return 0;
}
