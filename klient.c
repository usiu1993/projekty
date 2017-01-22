#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#define BUF_SIZE 100
struct timespec czasProc;
int fpipe[2];
char czas[40];
char buffer1[255];
int Ppid;
long iterator=0;

void komunikat(int sig) {
	//pipe(fpipe);
printf("JEST w SIGU\n");	
	FILE *fileptr= fopen("plik.txt", "rb");
if(getpid()==Ppid){
			//close(fpipe[0]);
			//pipe(fpipe);
			fseek(fileptr,iterator-1, SEEK_SET);
			fread(buffer1, 1, 1, fileptr);  	
			//clock_gettime(CLOCK_REALTIME, &czasProc);
			printf("buff %s\n",buffer1);
			write(fpipe[1], buffer1, 1);
			//close(fpipe[1]);
			iterator++;
		

	}
	alarm(3);       
	signal(SIGALRM, komunikat);
}




int main(int argc, char *argv[])
{
    //struct sockaddr_un svaddr, claddr;
    //int sfd, j;
    struct sockaddr_un svaddr[10], claddr[10];
	int* sfd=malloc(10*sizeof(sfd));
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];
    int DZIECKO=0;
   
    Ppid=getpid();
	printf("parent pid :%d\n",Ppid);
	 
	int pid;
	
	pipe(fpipe);
	signal(SIGALRM, komunikat);
	alarm(2);
	
	

	DZIECKO=1;
    char *abstract_client;
    char** abstract_server;
    //char *abstract_server;

    //if (argc < 2 || strcmp(argv[1], "--help") == 0)
        //printf("argv prob \n");

    /* Create client socket; bind to unique pathname (based on PID) */

for(int i=0;i<2;i++){
    sfd[i] = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd[i] == -1)
        printf("sock prob\n");
        
    abstract_server= malloc(10*sizeof(*abstract_server));
   abstract_server[i] = malloc(sizeof *abstract_server[i] * 20);   

    abstract_client = "viper_client";
    //abstract_server = "viper_server2";
    
    if(i==0)
		strcpy(abstract_server[i],"viper_server21");
	if(i==1)
		strcpy(abstract_server[i],"viper_server2");

    memset(&claddr[i], 0, sizeof(struct sockaddr_un));
    claddr[i].sun_family = AF_UNIX;
    strncpy(&claddr[i].sun_path[1], abstract_client, strlen(abstract_client)); 

    if (bind(sfd[i], (struct sockaddr *) &claddr[i], 
      sizeof(sa_family_t) + strlen(abstract_client) + 1) == -1)
        printf("bind prob\n");

    /* Construct address of server */

    memset(&svaddr[i], 0, sizeof(struct sockaddr_un));
    svaddr[i].sun_family = AF_UNIX;
    strncpy(&svaddr[i].sun_path[1], abstract_server[i], strlen(abstract_server[i]));
}
		for(int i = 0; i<1; i++) {
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

	
    /* Send messages to server; echo responses on stdout */
while(1){
		
	 if(getpid()!=Ppid){ 
		 
		printf("jest tu %d\n",getpid()); 
		 
	struct timespec spi;
		spi.tv_sec=0;
		spi.tv_nsec=5000;
	
        close(fpipe[1]);
        char ot[2];
        
        // now read the data (will block) tu bylo 1 
        read(fpipe[0], ot,1);
        //printf("mam od %s\n",ot);
        nanosleep(&spi,NULL); 
        clock_gettime(CLOCK_REALTIME, &czasProc);
        snprintf(czas, 40, "%s |sec:%ld, nsec:%ld\n",ot,czasProc.tv_sec,czasProc.tv_nsec);
        printf("Child(%d) received value: %s\n", getpid(), ot);
        int wys=getpid()-Ppid;
		printf("wysyła %d <---\n",getpid()-Ppid);
		

	//	for(int i=0; i<2; i++)
		
			abstract_server= malloc(10*sizeof(*abstract_server));
			abstract_server[wys] = malloc(sizeof *abstract_server[wys] * 20);
			
			if(wys==1)
					strcpy(abstract_server[wys],"viper_server2");
				if(wys==2)
					strcpy(abstract_server[wys],"viper_server21");
			
			
       /* code FIX */
       if (sendto(sfd[wys], czas, 40, 0, (struct sockaddr *) &svaddr[wys],
                 (sizeof(sa_family_t) + strlen(abstract_server[wys]) + 1) ) != 40) 
              printf("fatal sendto");

        /* original - non working code - replaced with the code FIX above 
        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr,
                sizeof(struct sockaddr_un)) != msgLen)
        {
            fatal("sendto");
        } */

	
    memset(ot,0,1);
}
}
    //remove(claddr[i].sun_path);            /* Remove client socket pathname */
    exit(EXIT_SUCCESS);
}

