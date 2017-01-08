#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#define DANE 32
#define NAZWA 20
#define NR 3
#define errorExit(blad) { fprintf(stderr,blad); fprintf(stderr, " nr bledu: %d\n", errno); exit(EXIT_FAILURE); } 

	//Sygnał SIGPIPE gdy zamykamy odbiorcę
void sigpipe_handler(int signum){

	fprintf(stderr,"-> sygnał SIGPIPE %d od odbiorcy <-\n",signum);

}
	//Funkcja czytjąca z stdin i wysyłająca do aktywnych kanałów
void odczytZapis(int* kolejka,int iFifo)
{
	struct sigaction sigpipeS;
	sigpipeS.sa_handler = sigpipe_handler;
	sigpipeS.sa_flags   = 0;    
	sigemptyset(&sigpipeS.sa_mask); 
	sigaddset(&sigpipeS.sa_mask, SIGINT);
	ssize_t ilBajt;   
	char* bufor=(char*) malloc(DANE*sizeof(*bufor));

	for(;;) {
			//Czytanie wyjscia datownika
		while(read(STDIN_FILENO, bufor, DANE))
		{
			for(int i=0;i<iFifo;i++){

					if (sigaction(SIGPIPE, &sigpipeS, NULL) == -1)
						errorExit("sigaction error\n");
						//Przypadek 1 zamknięcie strumienia 
					if ((ilBajt = write(kolejka[i], bufor, DANE)) == -1)
					{
						fprintf(stderr,"Zerwano połączenie! wyrzucam kanał o deskryptorze %d\n",kolejka[i]);
						close(kolejka[i]);
						kolejka[i]=-1;
						for(int i=0;i<iFifo;i++)
						{		//Zaktualizowanie obsługiwanych kolejek
							if(kolejka[i]==-1)
							{

								for(int j=i;j<iFifo;j++)
									kolejka[j]=kolejka[j+1];
								iFifo--;

								if(iFifo==0)
									errorExit("Żaden z kanałów nie jest aktywny");
							}

						}

					}	//Przypadek 2 przepełnienie kolejki
					else if(ilBajt<DANE)
					{
						fprintf(stderr,"Blad odczytu, przepelnione fifo\n");
						close(kolejka[i]);
						kolejka[i]=-1;					
						fprintf(stderr,"wyrzucam kanał\n"); 
						for(int i=0;i<iFifo;i++)
						{		//Zaktualizowanie obsługiwanych kolejek
							if(kolejka[i]==-1)
							{
								for(int k=i;k<iFifo;k++)
									kolejka[k]=kolejka[k+1];
								iFifo--;
								if(iFifo==0)
								{
									errorExit("Żaden z kanałów nie jest aktywny\n");
								}
							}
						}
					}
			}
		}

		break;
	}           
}


int main(int argc, char** argv)
{	
	int DIAGNOSTIC =0;	
	int opt;
	int ilKolejek;
	char* nazwa=(char*) malloc(DANE*sizeof(*nazwa));
	char* nr=(char*) malloc(NR*sizeof(*nr));
	char* nazwaPliku=(char*) malloc(DANE*sizeof(*nazwaPliku));
	int diag;

	while ((opt = getopt(argc, argv, "p:c:L:")) != -1) 
	{
		switch (opt) 
		{
			case 'p':
				nazwa=optarg;
				break;
			case 'c':
				ilKolejek=atoi(optarg);
				break;
			case 'L':
				DIAGNOSTIC=1;
				nazwaPliku=optarg;
				break;
			default:
				printf("Wymagane: -p<string> -c<int>\n");
				exit(1);

		}
	}
		//Jeżeli wybrano parametr -L, informacje diag. gromadzone w pliku
	if(DIAGNOSTIC)
	{
		diag = open(nazwaPliku, O_RDWR|O_CREAT|O_APPEND, 0600);
		if (-1 == diag) { errorExit("blad otwarcia pliku\n");}	

		if (-1 == dup2(diag, fileno(stderr))) { errorExit("blad przekierowania stderr\n");}
	}
	char *nazwaKol=(char*) malloc(NAZWA*sizeof(*nazwaKol));
	int *fd=(int*) malloc(ilKolejek*sizeof(*fd));
	strcpy(nazwaKol, nazwa);
	char **kolejki = malloc(sizeof *kolejki * ilKolejek);
	struct stat sBuf;
	for (int i = 0 ; i<ilKolejek; i++)
	{
		kolejki[i] = malloc(sizeof *kolejki[i] * NAZWA);
		sprintf(nr,"%d",i);
		strcat(nazwaKol, nr);
		memset (&sBuf, 0, sizeof (sBuf));
		stat(nazwaKol, &sBuf); 
			//sprawdzenie czy plik jest kolejką fifo
		if (S_ISFIFO(sBuf.st_mode))
			strcpy(kolejki[i],nazwaKol);
		else
			fprintf(stderr,"%s nie jest plikiem fifo\n",nazwaKol);

		strcpy(nazwaKol, nazwa);
	}
	int iloscAktyw=0;
	int* aktywneKol=(int*)malloc(1*sizeof(*aktywneKol));
	for(int k=0;k<ilKolejek;k++)
	{		//sprawdzenie czy plik jest otwarty do pisania
		fd[k]=open(kolejki[k],O_WRONLY |O_NONBLOCK);	
		if(fd[k]!=-1)
		{	fprintf(stderr,"%s otwarto do pisania, deskryptor: %d\n",kolejki[k],fd[k]);
			iloscAktyw++;
			aktywneKol=realloc(aktywneKol,iloscAktyw*sizeof(*aktywneKol));	
			aktywneKol[iloscAktyw-1]=fd[k];
		}
		else if(strcmp(kolejki[k],"")!=0){
			fprintf(stderr,"%s nie jest otwarta do pisania\n",kolejki[k]);
			close(fd[k]);
		}	
		
	}
	if(!iloscAktyw) 
		{
			fprintf(stderr,"Żadna kolejka nie jest otwarta do pisania\n");
			exit(EXIT_SUCCESS);
		}	
	
		//Wywołanie funkcji dla wyselekcjonowanych kolejek
	odczytZapis(aktywneKol,iloscAktyw);

	if(DIAGNOSTIC)		
		close(diag);

	fprintf(stderr,"\n===========================\n");
	fprintf(stderr,"Koniec dzialania programu. \n");
	fprintf(stderr,"===========================\n");
		//Zamknięcie pozostałych połączeń
	for(int i=0;i<iloscAktyw;i++)
		close(aktywneKol[i]);

		//Zwolnienie pamięci
	for(int k=0;k<ilKolejek;k++)
		free(kolejki[k]);

	free(kolejki);
	free(fd);
	free(nazwaKol);
	free(aktywneKol);	
	free(nr);
	return 0;
}
