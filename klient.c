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
#define SEND_STRLEN 11
#define BUF_SIZE 100
#define NAME_LEN 30
#define DL_WIAD 40
#define DL_NAZWY 35
#define SHA_DIGEST_LENGTH 20

#define SERVER_PATH "tpf_unix_sock.server1"
#define CLIENT_PATH "tpf_unix_sock.client1"
#define DATA "Hello from client"
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
int czasAl;

 char* f1(char* data,size_t len)
{	 char* out=malloc(30*sizeof(char));
	 char* output = malloc(20*sizeof(char));
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1((unsigned char*)data, len, hash);
  
   for(int i=0;i<SHA_DIGEST_LENGTH;i++){

        sprintf(output,"%02x",hash[i]);
        strcat(out,output);
    }

	return out;
	
	}

void sigquit_handler (int sig) {
    assert(sig == SIGQUIT);
    pid_t self = getpid();
    if (pidRodzica != self) _exit(0);
}



void sygnWys(int sig) {
		//unsigned char result[10];
		//wiadomosc="sak";
		//int al=2;
		
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
				write(fd, a, 40);
				czasAl=0;
				sleep(3);
				done=1;
				}

	}
	alarm(czasAl);      
	signal(SIGALRM, sygnWys);
}




int main(int argc, char *argv[])
{
	struct sockaddr_un adres,adresP;
  char buf[30];
  char buf2[100];
char* nazwaKan=malloc(DL_NAZWY*sizeof(char));
char* nazwaKanKl=malloc(DL_NAZWY*sizeof(char));
char* idSerw =malloc(NAME_LEN*sizeof(char));
char* idKli =malloc(NAME_LEN*sizeof(char)); 
char* otrzym=malloc(1*sizeof(char));
 char** kanalKlient;
    char** kanalPryw;
  int start=0;
  char* ilRobStr=malloc(4*sizeof(char));
  int bajtPipe;
  int ilRob;
   int ilBajt2;
   int fdP;
  int opt;
char* nazwaRej;
  //if (argc > 1) socket_path=argv[1];
if(argc<5)
	{
		printf("Argumenty wymagane: -m <kanal do rejestracji> -d <ilosc robotnikow>\n"
		"-w <nazwa pliku .txt>/b -c <czas>\n");
		exit(1);

	}


while ((opt = getopt(argc, argv, "m:d:w:c:")) != -1) 
	{
		switch (opt) 
		{
			case 'm':
			//ADRES KANALU DO REJESTRACJI
				nazwaRej=optarg;
				break;
			case 'd':
				ilRobStr=optarg;
				break;

			case 'w':
				wiadomosc=optarg;
				break;		
				
			case 'c':
				czasAl=atoi(optarg);
				break;		
					
			default:
				printf("Argumenty wymagane: -m <kanal do rejestracji> -d <ilosc robotnikow>\n"
		"-w <nazwa pliku .txt>/b -c <czas>\n");
				exit(1);
		}


	}








  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&adres, 0, sizeof(adres));
  adres.sun_family = AF_UNIX;
  
  strncpy(adres.sun_path, nazwaRej, sizeof(adres.sun_path)-1);

  if (connect(fd, (struct sockaddr*)&adres, sizeof(adres)) == -1) {
    perror("connect error");
    exit(-1);
  }
  printf("Wpisz Id brygady\n");
  while( (ilBajt=read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
	 
    if (write(fd, buf, 30) == 30) {
		 //read(STDIN_FILENO, buf2, sizeof(buf2));
				write(fd,ilRobStr,sizeof(ilRobStr));
						
					break;
				
	}
	else{	
        perror("write error");
        exit(-1);
    }
}


sleep(2);


ilRob=atoi(ilRobStr);



//Prywatny 
    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr; 
    struct sockaddr_un client_sockaddr; 
    char bufI[256];
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
     

client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        printf("SOCKET ERROR \n");
        exit(1);
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    client_sockaddr.sun_family = AF_UNIX;   
    strcpy(&client_sockaddr.sun_path[1], CLIENT_PATH); 
    len = sizeof(client_sockaddr);
    
    unlink(CLIENT_PATH);
    rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
    if (rc == -1){
        printf("BIND ERROR: \n");
        close(client_sock);
        exit(1);
    }
        
    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* for the server socket and connect   */
    /* to it.                              */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;
   // strcpy(server_sockaddr.sun_path, SERVER_PATH);
    strncpy(&server_sockaddr.sun_path[1], SERVER_PATH,strlen(SERVER_PATH));
    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    //rc = connect(client_sock, (struct sockaddr*) &server_sockaddr, offsetof(struct sockaddr_un, sun_path) + 1/*\0*/ + strlen(SERVER_PATH));
    if(rc == -1){
        printf("CONNECT ERROR \n" );
        close(client_sock);
        exit(1);
    }
    
    /************************************/
    /* Copy the data to the buffer and  */
    /* send it to the server socket.    */
    /************************************/
   // strcpy(bufI, DATA);                 
   // printf("Sending data...\n");








//--------------------------------------------------------------------------

start=1;



if(start)
{
	signal(SIGQUIT, sigquit_handler);
	
    struct sockaddr_un adrSerw[ilRob+1], adrKli[ilRob+1];
	sfd=malloc((ilRob+1)*sizeof(sfd));

		int dlugosc = strlen(buf);
    if (dlugosc > 0 && buf[dlugosc-1] == '\n') 
			buf[dlugosc-1] = '\0';
	char* napis=malloc(dlugosc*sizeof(char));
	napis = buf;
	int A=strlen(napis)+1;
	printf("A to %d\n",A);
	idSerw="viper_serv";
	idKli="viper_clint";
    pidRodzica=getpid();
	printf("parent pid :%d\n",pidRodzica);
	 
	int pid;
	
	pipe(fpipe);
	signal(SIGALRM, sygnWys);
	alarm(czasAl);
	//ualarm(900000,900000);  
	
   

    /* Create client socket; bind to unique pathname (based on PID) */

for(int i=0;i<ilRob+1;i++){
    sfd[i] = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd[i] == -1)
       perror("socket error");
   // else
		//printf("zrobiono socket %d\n",i);
        
    kanalPryw= malloc((ilRob+1)*sizeof(*kanalPryw));
    kanalPryw[i] = malloc(sizeof *kanalPryw[i] * DL_NAZWY);   
	snprintf(nazwaKan,DL_NAZWY,"%s%d",napis,i);
	kanalPryw[i]=nazwaKan;
	printf("%s",kanalPryw[i]);
	kanalKlient= malloc((ilRob+1)*sizeof(*kanalKlient));
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
        printf("Robotnik (%d): %d\n", i + 1, getpid());
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

			//snprintf(datagram, DL_WIAD, "%s |sec:%ld, nsec:%ld\n",otrzym,czasProc.tv_sec,czasProc.tv_nsec);
        snprintf(datagram, DL_WIAD, "%s |sec:%ld, nsec:%ld\n",otrzym,czasProc.tv_sec,czasProc.tv_nsec);
        printf("Robotnik(%d) received value: %s\n", getpid(), otrzym);
			 int wys=getpid()-pidRodzica-1;
			 
       if (sendto(sfd[wys], datagram, DL_WIAD, 0, (struct sockaddr *) &adrSerw[wys],
                 (sizeof(sa_family_t) + A+ 1) ) != DL_WIAD) 
              perror("sendto");
       else{
			printf("wyslal %d zawodnik\n",wys);
		}
	
    memset(otrzym,0,1);
    
}
}
char ww[100]="Koniec dzialania brygady.\n";
char* selection=malloc(1);
socklen_t len;

  printf("Sending data...\n");
    rc = send(client_sock, ww, strlen(ww), 0);
    if (rc == -1) {
        printf("SEND ERROR = n" );
        close(client_sock);
        exit(1);
    }   
    else {
        printf("Data sent!\n");
    }
	printf("Zapytac o sume kontrolna? [y/n]\n");
	//while('\n' != getchar())
	//{}
	scanf("%c", selection);
	if(*selection=='y')
	{
		printf("Sending data...\n");
   int rc2 = send(client_sock, selection, strlen(selection), 0);
    if (rc2 == -1) {
        printf("SEND ERROR selection" );
        close(client_sock);
        exit(1);
    } 
		
		char* o2=malloc(300*sizeof(char));
	int	rc3 = recv(client_sock, o2, 300,0);
    if (rc3 == -1) {
        printf("RECV ERROR \n");
        close(client_sock);
        exit(1);
    }   
    else {
        printf("DATA RECEIVED = %s\n", o2);
    }
		
		
	}
	else
		printf("nie\n");
	
    
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

