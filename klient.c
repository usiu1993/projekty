#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <openssl/sha.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <openssl/md5.h>
#include <assert.h>
#define ilRob 150
#define SEND_STRLEN 11
#define BUF_SIZE 100
#define NAME_LEN 30
#define DL_WIAD 40
#define DL_NAZWY 35
#define SHA_DIGEST_LENGTH 20
struct timespec czasProc;
int fpipe[2];
char datagram[40];
char buffer1[255];
int pidRodzica;
long iterator=0;
int *sfd;
char* wiadomosc;
char c;
int fd,ilBajt;
char buf3[5];
int done=0;


 char* f1(char* data,size_t len)
{	 char* out=malloc(30*sizeof(char));
	 char* output = malloc(20*sizeof(char));
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1((unsigned char*)data, len, hash);
  
   for(int i=0;i<SHA_DIGEST_LENGTH;i++){

      // printf("%02x", hash[i]);
        sprintf(output,"%02x",hash[i]);
        strcat(out,output);
    }
    
    //printf("%s\n",out);
	return out;
	
	}

void sigquit_handler (int sig) {
    assert(sig == SIGQUIT);
    pid_t self = getpid();
    if (pidRodzica != self) _exit(0);
}



void sygnWys(int sig) {
		//unsigned char result[10];
		wiadomosc="sak";
		int al=2;
		
		//char data[5] = "basdt";
size_t length = strlen(wiadomosc);
	char *a=malloc(40);
	 a=f1(wiadomosc,length);
		//output="ds";
			
					
		int dlugosc=strlen(wiadomosc);
		strcpy(buf3,"stop");
		
		
if(getpid()==pidRodzica){
			c=wiadomosc[iterator];
			snprintf(buffer1,sizeof(buffer1),"%c",c); 	
			printf("buff %s\n",buffer1);
			write(fpipe[1], buffer1, 1);
			iterator++;
			if(iterator==dlugosc)
				{
//				str2md5(wiadomosc, dlugosc);
				//printf("%s\n",a);
				write(fd, a, 40);
				//exit(1);
				al=0;
				sleep(3);
				done=1;
				}

	}
	alarm(al);       
	signal(SIGALRM, sygnWys);
}




int main(int argc, char *argv[])
{
	struct sockaddr_un adres;
  char buf[100];
char* nazwaKan=malloc(DL_NAZWY*sizeof(char));
char* nazwaKanKl=malloc(DL_NAZWY*sizeof(char));
char* idSerw =malloc(NAME_LEN*sizeof(char));
char* idKli =malloc(NAME_LEN*sizeof(char)); 
char* otrzym=malloc(1*sizeof(char));
 char** kanalKlient;
    char** kanalPryw;
  int start=0;
  int bajtPipe;
char* socket_path;
  if (argc > 1) socket_path=argv[1];




  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&adres, 0, sizeof(adres));
  adres.sun_family = AF_UNIX;
  
  strncpy(adres.sun_path, socket_path, sizeof(adres.sun_path)-1);

  if (connect(fd, (struct sockaddr*)&adres, sizeof(adres)) == -1) {
    perror("connect error");
    exit(-1);
  }
  while( (ilBajt=read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
	 
    if (write(fd, buf, ilBajt) == ilBajt) {
		break;
	}
	else{	
        perror("write error");
        exit(-1);
    }
}


sleep(2);

start=1;


if(start)
{
	signal(SIGQUIT, sigquit_handler);
	
    struct sockaddr_un adrSerw[ilRob], adrKli[ilRob];
	sfd=malloc(ilRob*sizeof(sfd));

	idSerw="viper_serv";
	idKli="viper_clint";
    pidRodzica=getpid();
	printf("parent pid :%d\n",pidRodzica);
	 
	int pid;
	
	pipe(fpipe);
	signal(SIGALRM, sygnWys);
	alarm(2);
	
   

    /* Create client socket; bind to unique pathname (based on PID) */

for(int i=0;i<ilRob;i++){
    sfd[i] = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd[i] == -1)
       perror("socket error");
   // else
		//printf("zrobiono socket %d\n",i);
        
    kanalPryw= malloc(ilRob*sizeof(*kanalPryw));
    kanalPryw[i] = malloc(sizeof *kanalPryw[i] * DL_NAZWY);   
	snprintf(nazwaKan,DL_NAZWY,"%s%d",idSerw,i);
	kanalPryw[i]=nazwaKan;
	printf("%s",kanalPryw[i]);
	kanalKlient= malloc(ilRob*sizeof(*kanalKlient));
    kanalKlient[i] = malloc(sizeof *kanalKlient[i] * DL_NAZWY);   
	snprintf(nazwaKanKl,DL_NAZWY,"%s%d",idKli,i);
	kanalKlient[i]=nazwaKanKl;
	printf("%s\n",kanalKlient[i]);
    memset(&adrKli[i], 0, sizeof(struct sockaddr_un));
    adrKli[i].sun_family = AF_UNIX;
    strncpy(&adrKli[i].sun_path[1], kanalKlient[i], strlen(kanalKlient[i])); 

    if (bind(sfd[i], (struct sockaddr *) &adrKli[i], 
      sizeof(sa_family_t) + strlen(kanalKlient[i]) + 1) == -1)
        perror("binding error");
  //  else
		//printf("zbindowany %s\n",kanalPryw[i]);

    /* Construct address of server */

    memset(&adrSerw[i], 0, sizeof(struct sockaddr_un));
    adrSerw[i].sun_family = AF_UNIX;
    strncpy(&adrSerw[i].sun_path[1], kanalPryw[i], strlen(kanalPryw[i]));
}
		for(int i = 0; i<ilRob; i++) {
    pid = fork();
    if(pid < 0) {
        perror("fork error");
        exit(-1);
    } else if (pid == 0) {
        printf("Robottnik (%d): %d\n", i + 1, getpid());
        break; 
    }  
}

    /* Send messages to server; echo responses on stdout */
while(!done){		
	 if(getpid()!=pidRodzica){ 
		struct timespec spi;
		spi.tv_sec=0;
		spi.tv_nsec=1000000;
	
        close(fpipe[1]);

        //int wys;
        // now read the data (will block) tu bylo 1 
        bajtPipe=read(fpipe[0], otrzym,1);
        if(bajtPipe==0)
			break;
		else if(bajtPipe==-1)
		{
		perror("read from pipe error");
        exit(-1);
			}
        nanosleep(&spi,NULL); 
        clock_gettime(CLOCK_REALTIME, &czasProc);
        snprintf(datagram, DL_WIAD, "%s |sec:%ld, nsec:%ld\n",otrzym,czasProc.tv_sec,czasProc.tv_nsec);
        printf("Robotnik(%d) received value: %s\n", getpid(), otrzym);
			 int wys=getpid()-pidRodzica-1;
			 
       if (sendto(sfd[wys], datagram, DL_WIAD, 0, (struct sockaddr *) &adrSerw[wys],
                 (sizeof(sa_family_t) + SEND_STRLEN + 1) ) != DL_WIAD) 
              perror("sendto");
       else{
			printf("wyslal %d zawodnik\n",wys);
		}
	
    memset(otrzym,0,1);
    
}
}
    
	}
	
	
    kill(-pidRodzica, SIGQUIT);
    for (int i = 0; i < ilRob-1; ++i) {
        int status;
        while (1) {
            pid_t child = wait(&status);
            if (child > 0 && WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("Robotnik %d zakończyl prace\n", (int)child);
            } else if (child < 0 && errno == EINTR) {
                continue;
            } else {
                break;
            }
		}
	}
	//PAMIEC
	
	printf("jest tu\n");
 	for(int k=0;k<ilRob;k++)
 	{
		free(kanalPryw[k]);
	free(kanalKlient[k]);
	}
	free(kanalPryw);
	free(kanalKlient);
	
	free(nazwaKan);
	free(nazwaKanKl);
	//free(idSerw);
	//free(idKli);
	free(otrzym);
	
	printf("zwolnil\n");
               /* Remove client socket pathname */
    exit(EXIT_SUCCESS);
}

