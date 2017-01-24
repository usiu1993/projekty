#include <openssl/sha.h>	
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
 #include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <openssl/md5.h>
#include <sys/un.h>
#define DL_WIAD 40
#define CH_SUM 40	
#define NAME_LEN 50
#define DL_NAZWY 35
#define DL_WZORCA 30
#define SHA_DIGEST_LENGTH 20

ssize_t bajtOdebr;
char **wiadomosc;
char **otrzymDat;


volatile sig_atomic_t done = 0;


 char* sumaKontr(char* data,size_t len)
{	
	char* suma=malloc(CH_SUM*sizeof(char));
	char* pojHash = malloc(20*sizeof(char));
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1((unsigned char*)data, len, hash);
  
   for(int i=0;i<SHA_DIGEST_LENGTH;i++){
        sprintf(pojHash,"%02x",hash[i]);
        strcat(suma,pojHash);
    }
    free(pojHash);
	return suma;
	
	}

 
 int porownanie (char **str1, char **str2)
{
	//if(strlen(*str1)==strlen(*str2))
		return strcmp(*(str1)+13,*(str2)+13);
	//else if(strlen(*(str1)+13)>strlen(*(str2)+13))
	//		return 1;
	//else 
	//	return -1;
}

 
void Sortuj(char ** tablica,int rozmiar)
{
	
	qsort (tablica,rozmiar,sizeof(char *),
                    (int (*)(const void *, const void *)) porownanie);
                    
}


int main(int argc, char *argv[])
{		
  struct sockaddr_un adres;
  struct timespec spi;
	
  char** kanalPryw;
  char* kanalRej=malloc(NAME_LEN*sizeof(char));
  int brygFd,bajtCzyt,bajtCzyt2;
  int fd;
  int flagi;
	int datagram =0;
	char* otrzymSH1=malloc(CH_SUM*sizeof(otrzymSH1));
	char* tablica=malloc(DL_WIAD*sizeof(char));
	char* wysBajt=malloc(1*sizeof(char));
	char* odebrWiad=malloc(DL_WIAD*sizeof(char));
	char* nazwKan=malloc(DL_NAZWY*sizeof(char));
	int ilRob;
	char* nazwaRej;
	int ilosc=0;
	char* liczbaRob=malloc(4*sizeof(char));
	char *sumaSH1=malloc(CH_SUM*sizeof(char));
	int opt;
	
  if (argc < 2) {
				printf("Wymagane -m <nazwa kanalu do rejestracji>");
				exit(1);
	  };
  
  while ((opt = getopt(argc, argv, "m:")) != -1) 
	{
		switch (opt) 
		{
			case 'm':
				nazwaRej=optarg;
				break;	
			default:
				printf("Wymagane -m <nazwa kanalu do rejestracji>\n");
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
    unlink(nazwaRej);
  

  if (bind(fd, (struct sockaddr*)&adres, sizeof(adres)) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(fd, 5) == -1) {
    perror("listen error");
    exit(-1);
  }
	
	

  while (1) {
    if ( (brygFd = accept(fd, NULL, NULL)) == -1) {
      perror("accept error");
       exit(-1);
    }
    /*Rejestracja brygady*/
	printf("Podaj ID brygady:\n");
    if ( (bajtCzyt=read(brygFd,kanalRej,30)) > 0) {
     printf("Zarejestrowano brygade: %s\n", kanalRej);
		}

   
    if (bajtCzyt == -1) {
      perror("read error");
      exit(-1);
    }
    else if (bajtCzyt == 0) {
      printf("EOF\n");
      close(brygFd);
    }
    
    /*Pobranie liczby robotnikow*/
     if(read(brygFd,liczbaRob,sizeof(liczbaRob))>0){
				printf("liczba rob %s\n",liczbaRob);
				 break;
		}
	
    
    
    
  }
	ilRob=atoi(liczbaRob);
	
	/*Alokacja pamieci na dekryptory dla wszystkich robotnikow*/
    int* sfd=malloc((ilRob+1)*sizeof(sfd));
    
	/*Tworzenie kanalu prywatnego dla Brygadzisty*/
	int serwPryw, klPryw;
	int rc;
	socklen_t len;
    struct sockaddr_un serwerP;
    struct sockaddr_un klientP;
    int bajtyOt = 0;     
    int nasluch = 1;
    memset(&serwerP, 0, sizeof(struct sockaddr_un));
    memset(&klientP, 0, sizeof(struct sockaddr_un));
             
    /*Tworzenie nazwy kanalow na podstawie wprowadzonej nazwy*/
    		int dlugosc = strlen(kanalRej);
   if (dlugosc > 0 && kanalRej[dlugosc-1] == '\n') 
			kanalRej[dlugosc-1] = '\0';
	char* nazwa=malloc(dlugosc*sizeof(char));
	 strcpy(nazwa,kanalRej);
    
    /* Kanal w domenie Unix */
    serwPryw = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serwPryw == -1){
        perror("socket error");
       exit(-1);
    }

    /*Wypelnienie struktur, bindowanie adresu prywatnego*/
    serwerP.sun_family = AF_UNIX;   
    len = sizeof(serwerP);
     strncpy(&serwerP.sun_path[1], nazwa,strlen(nazwa));
     unlink(nazwa);
    rc = bind(serwPryw, (struct sockaddr *) &serwerP, len);
    if (rc == -1){
        perror("binding error");
        close(serwPryw);
        exit(-1);
    }

    /* Nasluch na podanej liczbie kanalow */
    rc = listen(serwPryw, nasluch);
    if (rc == -1){ 
        perror("listen error");
        close(serwPryw);
        exit(-1);
    }

    /*Stworzenie polaczenia dla gniazda*/
    klPryw = accept(serwPryw, (struct sockaddr *) &klientP, &len);
    if (klPryw == -1){
        perror("accept error");
        close(serwPryw);
        close(klPryw);
        exit(-1);
    }
    
    /*Rozpoczecie komunikacji datagramowej*/	
	datagram=1;
	
	if(datagram){
		
   struct sockaddr_un svaddr[ilRob], claddr[ilRob];
   wiadomosc=malloc(ilRob*sizeof(*wiadomosc));
   socklen_t len;
  
  /*Tworzenie kanalow dla wszystkich robotnikow wg wzorca*/
   for(int i=0;i<ilRob;i++)
   {
   
   kanalPryw= malloc((ilRob)*sizeof(*kanalPryw));
   kanalPryw[i] = malloc(sizeof *kanalPryw[i] * DL_NAZWY);
	snprintf(nazwKan,DL_NAZWY,"%s%d",nazwa,i);
	printf("%s\n",nazwKan);
	kanalPryw[i]=nazwKan;

	/* Nowe sockety dla datagramow*/
   sfd[i] = socket(AF_UNIX, SOCK_DGRAM, 0);       /* Create server socket */
   if (sfd[i] == -1)
      perror("creating socket error ");

  
   memset(&svaddr[i], 0, sizeof(struct sockaddr_un));
   svaddr[i].sun_family = AF_UNIX;
   /*Kanaly prywatne*/
   strncpy(&svaddr[i].sun_path[1], kanalPryw[i], strlen(kanalPryw[i]));
   
	/* Bindowanie serwera do adresu */	
   if (bind(sfd[i], (struct sockaddr *) &svaddr[i], 
     sizeof(sa_family_t) + strlen(kanalPryw[i]) + 1) == -1)
       {
			perror("binding error ");
			exit(-1);
		   }
	/*Dla kazdego robotnika osobna tblica wiadomosci*/
   wiadomosc[i]=malloc(sizeof*wiadomosc[i]*DL_WIAD);
}

   while (!done) {
	    
	    /*Odbieranie wiadomosci od robotnikow */
	   for(int i=0;i<ilRob;i++)
	    {
				
       len = sizeof(struct sockaddr_un);
       bajtOdebr = recvfrom(sfd[i], wiadomosc[i], DL_WIAD, MSG_DONTWAIT ,
                           (struct sockaddr *) &claddr[i], &len);                   
                     
        /*Naraz pisze jeden robotnik*/                   
       if (bajtOdebr < 0)
       {	continue;
	   }
		if(bajtOdebr>0)
		{
		ilosc++;
		otrzymDat = (char**)realloc(otrzymDat, (ilosc+1)*sizeof(*otrzymDat));
		otrzymDat[ilosc-1] = (char*)malloc(40);
		strcpy(otrzymDat[ilosc-1], wiadomosc[i]);
		printf("Archiwista otrzymal: %s", wiadomosc[i]);		
		}
		
		/*Otrzymanie sumy kontrolnej do poźniejszego porownania*/
		flagi = fcntl(brygFd, F_GETFL, 0);
		fcntl(brygFd, F_SETFL, flagi | O_NONBLOCK);
		
		if ( (bajtCzyt2=read(brygFd,otrzymSH1,CH_SUM)) > 0)
		{
			printf("otrzymano sumę kontrolną: %s\n",otrzymSH1);
			close(brygFd);
			
			/*Zamykanie wszystkich kanalow*/
			for(int i=0;i<ilRob;i++)
				close(sfd[i]);		

			/*Sortowanie otrzymanych bajtow wzgledem stempla*/
			Sortuj(otrzymDat,ilosc);

	/*Sklejanie otrzymanej wiadomosci*/
	for(int i=0;i<ilosc;i++){
		sprintf(tablica,"%c",*otrzymDat[i]);
		sprintf(wysBajt,"%c",tablica[0]);
		strcat(odebrWiad,wysBajt);
	}
	/*Obliczenie sumy SHA1*/
	sumaSH1=sumaKontr(odebrWiad,strlen(odebrWiad));
			
	printf("Odebrna wiadomosc: %s\n",odebrWiad);
	spi.tv_sec=2;
	spi.tv_nsec=0;
	nanosleep(&spi,NULL);
	done=1;
		}
		}
		
     }
     
     int kon=0;
    char ww[30];
     while (!kon) {	 
	/*Odczytanie wiadomosci o stanie brygady*/	 
	  printf("Czytanie...\n");
	   memset(ww, 0, sizeof(ww));
    bajtyOt = recv(klPryw, ww, sizeof(ww),0);
    if (bajtyOt == -1){
        perror("recv error: \n");
        close(serwPryw);
        close(klPryw);
        exit(1);
    }
    else {
        printf("Otrzymano komunikat: %s\n", ww);
    }
	/*Realizacja zadania sumy kontrolnej*/
		char* o=malloc(1);
		 memset(o, 0, 1);
		int	bytes_rec2 = recv(klPryw, o, sizeof(o), 0);
	
		if(bytes_rec2>0)
		{
			if(*o=='y')
			{
				char suma[100];
				memset(suma, 0, 100);
				/*porownianie sum kontrolnych*/
				if(strcmp(sumaSH1,otrzymSH1)==0){
				snprintf(suma,100,"suma %s\nJest zgodna z wczeniej przeslana\n",sumaSH1);
				}
				else{
					snprintf(suma,100,"suma %s\nNie jest zgodna\n z wczeniej przeslana\n",sumaSH1);
					}
				/*Przeslanie rezultatu do brygadizsty*/	
				 rc = send(klPryw, suma, strlen(suma), 0);
				
				}		
		}
		
		free(o);
     kon=1;
     
    }
 }
 /*Zwalnianie pamieci*/
 	for(int k=0;k<ilRob;k++)
 	{
		free(kanalPryw[k]);
		free(wiadomosc[k]);
		close(sfd[k]);
	
	}
	for(int k=0;k<=ilosc;k++)
		free(otrzymDat[k]);
		
	free(wiadomosc);
	free(nazwa);
	free(otrzymDat);
	free(otrzymSH1);
	free(kanalRej);
	free(nazwKan);
	free(sfd);
	free(tablica);
	free(wysBajt);
	free(odebrWiad);
 return 0;
}
 
 
 
