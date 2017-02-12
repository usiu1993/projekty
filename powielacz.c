#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#define DANE 32
#define NAZWA 80
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
						{
			perror("sigaction error\n");
			exit(-1);
		}
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
						fprintf(stderr,"Blad odczytu, przepelnione fifo! wyrzucam kanał odeskryptorze %d\n",kolejka[i]);
						close(kolejka[i]);
						kolejka[i]=-1;					
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


int diagnostyka(char* nazwa)
{
		int d = open(nazwa, O_RDWR|O_CREAT|O_APPEND, 0600);
		if (-1 == d) {{
			perror("file open error\n");
			exit(-1);
		}}	

		if (-1 == dup2(d, fileno(stderr))) { {
			perror("redirect error\n");
			exit(-1);
		}}
		
		return d;
	
}

int main(int argc, char** argv)
{	
	int DIAGNOSTIC=0;	
	char nazwa[DANE];
	char nr[NR];
	char nazwaPliku[DANE];	
	char nazwaKol[NAZWA];

	struct stat sBuf;
	int diag;
	int opt;
	int ilKolejek;

	while ((opt = getopt(argc, argv, "p:c:L:")) != -1) 
	{
		switch (opt) 
		{
			case 'p':
				strcpy(nazwa,optarg);
				break;
			case 'c':
				ilKolejek=strtol(optarg,NULL,0);
				break;
			case 'L':
				DIAGNOSTIC=1;
				strcpy(nazwaPliku,optarg);
				break;
			default:
				printf("Wymagane: -p<string> -c<int> opcjonalne L<string>\n");
				exit(1);

		}
	}
	
	int fd[ilKolejek];
	char kolejki[ilKolejek][NAZWA];
	
		//Jeżeli wybrano parametr -L, informacje diag. gromadzone w pliku
	if(DIAGNOSTIC)
		diag=diagnostyka(nazwaPliku);

	strcpy(nazwaKol, nazwa);
	
	for (int i = 0 ; i<ilKolejek; i++)
	{
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
	int* aktywneKol=(int*)malloc(sizeof(int));
	for(int k=0;k<ilKolejek;k++)
	{		//sprawdzenie czy plik jest otwarty do pisania
		fd[k]=open(kolejki[k],O_WRONLY);	
		if(fd[k]!=-1)
		{	fprintf(stderr,"%s otwarto do pisania, deskryptor: %d\n",kolejki[k],fd[k]);
			iloscAktyw++;
			aktywneKol=realloc(aktywneKol,iloscAktyw*sizeof(*aktywneKol));	
			aktywneKol[iloscAktyw-1]=fd[k];
		}
		else if(strcmp(kolejki[k],"")!=0){
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
		
	free(aktywneKol);	
	return 0;
}
