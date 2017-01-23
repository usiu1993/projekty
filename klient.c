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
#define ilRob 4
#define BUF_SIZE 100
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
int fd,rc;
char buf3[5];

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





void sygnWys(int sig) {
		//unsigned char result[10];
		wiadomosc="ajprld";
		
		
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
				printf("%s\n",a);
				write(fd, a, 40);
				exit(1);
				}

	}
	alarm(3);       
	signal(SIGALRM, sygnWys);
}




int main(int argc, char *argv[])
{
	//char datas[5] = "japrld";
/*size_t length = strlen(datas);
	char *a=malloc(30);
	 a=f1(datas,length);
	 printf("%s",a);
	*/
//=======================================================
//======================================================
struct sockaddr_un addr;
  char buf[100];
  
 //char* a="basdt";
//		char *output1 = str2md5(a, sizeof(a));
    //    printf("%s\n", output1);
 
 
  
  int start=0;
char* socket_path;
  if (argc > 1) socket_path=argv[1];

  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }

  while( (rc=read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    if (write(fd, buf, rc) == rc) {
		//close(fd);
		break;
	}
	else{	
      if (rc > 0) fprintf(stderr,"partial write");
      else {
        perror("write error");
        exit(-1);
      }
    }
}


sleep(3);

start=1;


if(start)
{

//=======================================================
//=======================================================
    struct sockaddr_un adrSerw[10], adrKli[10];
	sfd=malloc(10*sizeof(sfd));
	char* str=malloc(20);
	char* str2=malloc(20);
	char* idSerw = "viper_serv";
	char* idKli = "viper_clint";
	
    pidRodzica=getpid();
	printf("parent pid :%d\n",pidRodzica);
	 
	int pid;
	
	pipe(fpipe);
	signal(SIGALRM, sygnWys);
	alarm(2);
	
    char** abstract_client;
    char** abstract_server;

    /* Create client socket; bind to unique pathname (based on PID) */

for(int i=0;i<ilRob;i++){
    sfd[i] = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd[i] == -1)
        printf("sock prob\n");
    else
		printf("zrobiono socket %d\n",i);
        
    abstract_server= malloc(10*sizeof(*abstract_server));
    abstract_server[i] = malloc(sizeof *abstract_server[i] * 20);   
	snprintf(str,15,"%s%d",idSerw,i);
	abstract_server[i]=str;
	printf("%s",abstract_server[i]);
	abstract_client= malloc(10*sizeof(*abstract_client));
    abstract_client[i] = malloc(sizeof *abstract_client[i] * 20);   
	snprintf(str2,15,"%s%d",idKli,i);
	abstract_client[i]=str2;
	printf("%s\n",abstract_client[i]);
	printf("dl %d %lu\n",i,strlen(abstract_server[i]));
   
    memset(&adrKli[i], 0, sizeof(struct sockaddr_un));
    adrKli[i].sun_family = AF_UNIX;
    strncpy(&adrKli[i].sun_path[1], abstract_client[i], strlen(abstract_client[i])); 

    if (bind(sfd[i], (struct sockaddr *) &adrKli[i], 
      sizeof(sa_family_t) + strlen(abstract_client[i]) + 1) == -1)
        printf("bind prob\n");
    else
		printf("zbindowany %s\n",abstract_server[i]);

    /* Construct address of server */

    memset(&adrSerw[i], 0, sizeof(struct sockaddr_un));
    adrSerw[i].sun_family = AF_UNIX;
    strncpy(&adrSerw[i].sun_path[1], abstract_server[i], strlen(abstract_server[i]));
}
		for(int i = 0; i<ilRob; i++) {
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
		
	 if(getpid()!=pidRodzica){ 
		 
		printf("jest tu %d\n",getpid());
		sleep(2); 
		 
		struct timespec spi;
		spi.tv_sec=0;
		spi.tv_nsec=5000;
	
        close(fpipe[1]);
        char ot[2];
        //int wys;
        // now read the data (will block) tu bylo 1 
        if(read(fpipe[0], ot,1)==0)
			break;
        printf("mam od %s\n",ot);
        nanosleep(&spi,NULL); 
        clock_gettime(CLOCK_REALTIME, &czasProc);
        snprintf(datagram, 40, "%s |sec:%ld, nsec:%ld\n",ot,czasProc.tv_sec,czasProc.tv_nsec);
        printf("Child(%d) received value: %s\n", getpid(), ot);
			 int wys=getpid()-pidRodzica-1;

		printf("jest tu\n");
       if (sendto(sfd[wys], datagram, 40, 0, (struct sockaddr *) &adrSerw[wys],
                 (sizeof(sa_family_t) + 11 + 1) ) != 40) 
              perror("sendto");
       else{
			printf("wyslal %d zawodnik\n",wys);
		}
	
    memset(ot,0,1);
    
}
}
    
	}
               /* Remove client socket pathname */
    exit(EXIT_SUCCESS);
}

