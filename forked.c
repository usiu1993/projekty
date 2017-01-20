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



char * server_filename = "/tmp/socket-server";
char * client_filename = "/tmp/socket-client";

int Ppid;
//int fd[10][2];
int fd[2];
char buffer1[255];
char czas[70];    
struct timespec czasProc;
long iterator=0;

void komunikat(int sig) {
	FILE *fileptr= fopen("plik.txt", "rb");
if(getpid()==Ppid){
	//while(1){
      //for(int i=0; i<10; i++)
      //close(fd[0]);
			fseek(fileptr, iterator-1, SEEK_SET);
			fread(buffer1, 3, 1, fileptr);  
			//printf("buffer to %s\n",buffer1);
			clock_gettime(CLOCK_REALTIME, &czasProc);
			//snprintf(czas, 70, "%s |sec:%ld, nsec:%ld\n",buffer1,czasProc.tv_sec,czasProc.tv_nsec);
			//write(fd[i][1], buffer1, 1);
			write(fd[1], buffer1, 1);
			iterator++;
		

	}
alarm(3);    
	//alarm(1);    
	signal(SIGALRM, komunikat);
}



int main(int argc, char *argv[])
{

char* nazwaRej;
int ilRob;
int opt;
char * kom;
int czasWys;
int ROBOTNIK=1;
char* idBryg;

/*
if(argc<6)
	{
		printf("Argumenty wymagane: -m<kanal do rejestracji> -d<ilosc robotnikow>\n-w<nazwa pliku .txt>/b -c<czas> -p<ID>");
		exit(1);

	}



while ((opt = getopt(argc, argv, "m:d:w:c:p:")) != -1) 
	{
		switch (opt) 
		{
			case 'm':
			//ADRES KANALU DO REJESTRACJI
				nazwaRej=optarg;
				printf("%s",nazwaRej);
				break;
			case 'd':
				ilRob=atoi(optarg);
				break;

			case 'w':
				kom=optarg;
				break;		
				
			case 'c':
				czasWys=atoi(optarg);
				break;		
				
			case 'p':
				idBryg=optarg;
				break;		
			default:
				printf("Wymagane -m[sec] -f(fifo)/s(pipe)\n");
				exit(1);
		}


	}
*/
	    int s;
    char obuf[100];
    //===============================================================================
   
   //================================================================================ 
    if(ROBOTNIK){
    
    struct sockaddr_un srv_un, cli_un = { 0 };
    //----
    memset(&srv_un,0,sizeof(struct sockaddr_un));
    //---------
    srv_un.sun_family = AF_UNIX;
  
	strncpy(&srv_un.sun_path[1],server_filename ,strlen(server_filename));
	
	
    cli_un.sun_family = AF_UNIX;
    strncpy(&cli_un.sun_path[1], client_filename, strlen(client_filename));
    unlink(cli_un.sun_path);

    if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket server");
        exit(1);
    }
    
   // if (bind(s, (struct sockaddr *)&cli_un, sizeof(cli_un)) == -1) {
   if (bind(s, (struct sockaddr *)&cli_un, sizeof(sa_family_t)+strlen(client_filename)+1) == -1) {
        perror("bind client");
        exit(1);
    }

   // if (connect(s, (struct sockaddr *) &srv_un, sizeof(srv_un)) == -1) {
   if (connect(s, (struct sockaddr *) &srv_un, sizeof(sa_family_t)+strlen(client_filename)+1) == -1) {
        perror("connect client");
        exit(1);
    }
	
	
	
	
	int IMP=1;
	 
    char buffer12[255];
    long filelen;

	 Ppid=getpid();
	printf("parent pid :%d\n",Ppid);
    // create pipe descriptors
    //for(int i =0; i<10; i++)
		pipe(fd);

	int pid;
	int i;
	//char wys[255]="costam";


 signal(SIGALRM, komunikat);
	alarm(3);
//ualarm(500,500);
if(IMP!=0)
{
	printf("JEST TU DO CHUJA\n");
for(i = 0; i<3; i++) {
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
		
		//int diff=getpid()-Ppid;
		//printf("tu dziecko o %d a roznica to %d\n",getpid(),diff);
	
	
	
        close(fd[1]);
        char ot[2];
        
        // now read the data (will block) tu bylo 1 
        read(fd[0], ot,1);
        clock_gettime(CLOCK_REALTIME, &czasProc);
        snprintf(czas, 70, "%s |sec:%ld, nsec:%ld\n",ot,czasProc.tv_sec,czasProc.tv_nsec);
        printf("Child(%d) received value: %s\n", getpid(), ot);
        //snprintf(obuf, 100, "%s %s\n",ot,czas);
        //strcpy(obuf,ot);
        //printf("obuf : %s\n",obuf);
        //------------------------------------------------
           while(printf("> ")) {
		
			   
			   
        //printf("obuf %s\n",obuf);
        if (send(s, czas, sizeof(czas), 0) == -1) {
            perror("send");
            exit(1);
        }
        break;
        //close(fd[0]);
    }
        //------------------------------------------------
        memset(ot,0,1);
	}
  }


  close(s);
 //sleep(12);
}
    return 0;
}
