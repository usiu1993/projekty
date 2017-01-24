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
#define NAME_LEN 50
#define DL_WIAD 40
#define DL_NAZWY 35
#define SHA_DIGEST_LENGTH 20
#define KLIENT_SCIEZKA "klientSciezka"

struct timespec czasProc,sleep1;
int fpipe[2];
int pidRodzica;
long iterator=0;
int *sfd;
char* wiadomosc;
char c;
int fd,ilBajt;
int done=0;
int czasAl;

 char* ObliczSume(char* data,size_t len)
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
    if (pidRodzica != self) 
		exit(0);
}



void sygnWys(int sig) {
char wiadPipe[2];	
size_t length = strlen(wiadomosc);
	char *zwrotSumy=malloc(40*sizeof(char));
	 zwrotSumy=ObliczSume(wiadomosc,length);
		int dlugosc=strlen(wiadomosc);
		
if(getpid()==pidRodzica){
			c=wiadomosc[iterator];
			snprintf(wiadPipe,sizeof(wiadPipe),"%c",c); 	
			write(fpipe[1], wiadPipe, 1);
			iterator++;
			if(iterator==dlugosc)
				{
				write(fd, zwrotSumy, 40);
				czasAl=0;
				sleep1.tv_sec=2;
				sleep1.tv_nsec=0;
				nanosleep(&sleep1,NULL);
				done=1;
				}

	}
	alarm(czasAl);      
	signal(SIGALRM, sygnWys);
}




int main(int argc, char *argv[])
{
	struct sockaddr_un adres;
	struct timespec sleep2,sleep3,sleep4;
  char* buf=malloc(NAME_LEN*sizeof(char));
char* nazwaKan=malloc(DL_NAZWY*sizeof(char));
char* nazwaKanKl=malloc(DL_NAZWY*sizeof(char));
char* datagram=malloc(DL_WIAD*sizeof(char));
char* idKli =malloc(NAME_LEN*sizeof(char)); 
char* otrzym=malloc(1*sizeof(char));
 char** kanalKlient;
    char** kanalPryw;
  int start=0;
  char* ilRobStr=malloc(4*sizeof(char));
  int bajtPipe;
  int ilRob;
  int opt;
  int pid;
char* nazwaRej;

if(argc<5)
	{
		printf("Argumenty wymagane: -m <kanal do rejestracji> -d <ilosc robotnikow>\n"
		"-w <nazwa pliku .txt>/b -c <czas>(s)\n");
		exit(1);

	}


while ((opt = getopt(argc, argv, "m:d:w:c:")) != -1) 
	{
		switch (opt) 
		{
			case 'm':
				nazwaRej=optarg;
				break;
			case 'd':
				ilRobStr=optarg;
				break;

			case 'w':
				wiadomosc=optarg;
				if(strlen(wiadomosc)>DL_WIAD){
					printf("Za dluga wiadomosc\n");
					exit(1);
				}
				break;		
				
			case 'c':
				czasAl=atoi(optarg);
				break;		
					
			default:
				printf("Argumenty wymagane: -m <kanal do rejestracji> -d <ilosc robotnikow>\n"
		"-w <nazwa pliku .txt>/b -c <czas>(s)\n");
				exit(1);
		}


	}

	/* Tworzenie kanalu publicznego do rejestracji*/
  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&adres, 0, sizeof(adres));
  adres.sun_family = AF_UNIX;
	/*Kanal publiczny*/
  strncpy(adres.sun_path, nazwaRej, sizeof(adres.sun_path)-1);
	/*Połaczenie adresu z socketem*/
  if (connect(fd, (struct sockaddr*)&adres, sizeof(adres)) == -1) {
    perror("connect error");
    exit(-1);
  }
  /*Rejetracja nazwy brygady*/
  printf("Wpisz Id brygady\n");
  while( (ilBajt=read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
	 /*Przekazanie ilosci robotnikow*/
    if (write(fd, buf, 30) == 30) {
				write(fd,ilRobStr,sizeof(ilRobStr));				
					break;
				
	}
	else{	
        perror("write error");
        exit(-1);
    }
}

sleep2.tv_sec=2;
sleep2.tv_nsec=0;
nanosleep(&sleep2,NULL);
ilRob=atoi(ilRobStr);

/*Tworzenie wzorca nazw*/
	int dlugosc = strlen(buf);
char* napis=malloc(dlugosc*sizeof(char));	
    if (dlugosc > 0 && buf[dlugosc-1] == '\n') 
			buf[dlugosc-1] = '\0';
	 strcpy(napis,buf);

/*Tworzenie kanalu prywatnego dla Brygadisty*/ 
    int klienPryw, lb, len;
    struct sockaddr_un serwerP; 
    struct sockaddr_un klientP; 
    memset(&serwerP, 0, sizeof(struct sockaddr_un));
    memset(&klientP, 0, sizeof(struct sockaddr_un));
     
/*Przygotowanie socketu*/
klienPryw = socket(AF_UNIX, SOCK_STREAM, 0);
    if (klienPryw == -1) {
        perror("socket error: \n");
        exit(-1);
    }

    klientP.sun_family = AF_UNIX;
    /*Kanal bedzie kanalem prywatnym*/   
    strcpy(&klientP.sun_path[1], KLIENT_SCIEZKA); 
    len = sizeof(klientP);
    /*Na potrzeby bindowania*/
    unlink(KLIENT_SCIEZKA);
    /*Bindowanie adresu prywatnego*/
    lb = bind(klienPryw, (struct sockaddr *) &klientP, len);
    if (lb == -1){
        perror("bind error: \n");
        close(klienPryw);
        exit(-1);
    }

    /*Przygotowanie struktury dla domeny UNIX*/
    serwerP.sun_family = AF_UNIX;
    strncpy(&serwerP.sun_path[1], napis,strlen(napis));
    lb = connect(klienPryw, (struct sockaddr *) &serwerP, len);
    
    if(lb == -1){
        perror("connect error: \n" );
        close(klienPryw);
        exit(-1);
    }

/*Rozpoczecie komunikacji datagramowej*/
start=1;
if(start)
{
	signal(SIGQUIT, sigquit_handler);
	
    struct sockaddr_un adrSerw[ilRob], adrKli[ilRob];
	sfd=malloc((ilRob)*sizeof(sfd));

    pidRodzica=getpid();
	printf("PID Rodzica :%d\n",pidRodzica);
	/*Tworzenie pipa pomiedzy Brygadzista a robotnikami*/
	pipe(fpipe);
	/*Sygnal do przesylania kolejnych porcji*/
	signal(SIGALRM, sygnWys);
	alarm(czasAl);  

/*Dla kazdego robotnika osobny kanal prywatny*/
for(int i=0;i<ilRob;i++){
    sfd[i] = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd[i] == -1)
       perror("socket error");
      /*Tworzenie nazw kanalow wg wzorca*/      
    kanalPryw= malloc((ilRob)*sizeof(*kanalPryw));
    kanalPryw[i] = malloc(sizeof *kanalPryw[i] * DL_NAZWY);   
	snprintf(nazwaKan,DL_NAZWY,"%s%d",napis,i);
	kanalPryw[i]=nazwaKan;
	kanalKlient= malloc((ilRob)*sizeof(*kanalKlient));
    kanalKlient[i] = malloc(sizeof *kanalKlient[i] * DL_NAZWY);   
	snprintf(nazwaKanKl,DL_NAZWY,"%s%d",idKli,i);
	kanalKlient[i]=nazwaKanKl;
	
	/*Utworznie kanalow prywatnych w domenie UNIX*/
    memset(&adrKli[i], 0, sizeof(struct sockaddr_un));
    adrKli[i].sun_family = AF_UNIX;
    strncpy(&adrKli[i].sun_path[1], kanalKlient[i], strlen(kanalKlient[i])); 
    
    bind(sfd[i], (struct sockaddr *) &adrKli[i], 
      sizeof(sa_family_t) + strlen(kanalKlient[i]) + 1);

    /*Konstrukcja adresu serwera*/
    memset(&adrSerw[i], 0, sizeof(struct sockaddr_un));
    adrSerw[i].sun_family = AF_UNIX;
    strncpy(&adrSerw[i].sun_path[1], kanalPryw[i], strlen(kanalPryw[i]));
}

	/*Tworzenie robotnikow*/
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

while(!done){	
		/*Wysylaja tylko robotnicy*/
	 if(getpid()!=pidRodzica){ 
		 
		struct timespec spi;
		spi.tv_sec=0;
		spi.tv_nsec=1000000;
        close(fpipe[1]);
		/*Czytanie z pipe od Brygadzisty*/
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
        printf("Robotnik(%d) wysyla: %s\n", getpid(), otrzym);
		int wys=getpid()-pidRodzica-1;
		/*Robotnik który odczytal z pipe pisze do Archiwisty*/	 
       if (sendto(sfd[wys], datagram, DL_WIAD, 0, (struct sockaddr *) &adrSerw[wys],
                 (sizeof(sa_family_t) +strlen(napis)+1+ 1) ) != DL_WIAD) 
              perror("sendto");
    memset(otrzym,0,1);
}
}
char* ww=malloc(30*sizeof(char));
ww="Koniec dzialania";
char* selection=malloc(1);
/*Wyslanie komunikatu o zakonczeniu przesylu*/
  printf("Wysylam komunikat...\n");
    lb = send(klienPryw, ww, strlen(ww), 0);
    if (lb == -1) {
        printf("SEND ERROR = n" );
        close(klienPryw);
        exit(-1);
    }   
 /*Zapytanie o zgodnosc sum kontrolnych*/
	printf("Zapytac o sume kontrolna? [y/n]\n");
	/*Komunikacja z uzytkownikiem*/
	scanf("%c", selection);
	if(*selection=='y')
	{
		printf("Wysylanie...\n");
	/*Wyslanie decyzji o sumie kontorlnej*/	
   int rc2 = send(klienPryw, selection, strlen(selection), 0);
    if (rc2 == -1) {
        perror("send error of selection" );
        close(klienPryw);
        exit(-1);
    } 
	char* o2=malloc(100*sizeof(char));
	/*Odebranie sumy kontrolnej przeslanej wiadomosci*/
	int	rc3 = recv(klienPryw, o2, 100,0);
    if (rc3 == -1) {
        perror("recv error \n");
        close(klienPryw);
        exit(-1);
    }   
    else {
		sleep3.tv_sec=1;
		sleep3.tv_nsec=0;
		nanosleep(&sleep3,NULL);
        printf("Otzymano: %s\n", o2);
    }
		
		
	}  
	}
	
		sleep4.tv_sec=2;
		sleep4.tv_nsec=0;
		nanosleep(&sleep4,NULL);
	
	
	/*Brygadzista rozwiazuje brygade*/
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
	/*Zwalnianie pamieci*/
 	for(int k=0;k<ilRob;k++)
 	{
	free(kanalPryw[k]);
	free(kanalKlient[k]);
	}
	free(kanalPryw);
	free(kanalKlient);
	free(napis);
	free(nazwaKan);
	free(nazwaKanKl);
	free(datagram);
	
    return 0;
}

