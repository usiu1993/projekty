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
#define ilRob 150	
#define NAME_LEN 30
#define DL_NAZWY 35
#define SHA_DIGEST_LENGTH 20
	ssize_t bajtOdebr;

	char **wiadomosc;
	char **otrzymDat;


volatile sig_atomic_t done = 0;


 char* sumaKontr(char* data,size_t len)
{	//char* out=malloc(30*sizeof(char));
	//char* output = malloc(20*sizeof(char));
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
  return strcmp(*(str1)+13,*(str2)+13);
}

 
void Sortuj(char ** tablica,int rozmiar)
{
	
	qsort (tablica,rozmiar,sizeof(char *),
                    (int (*)(const void *, const void *)) porownanie);
                    
  // now display the sorted array
	//for (int i=0; i < rozmiar; i++) 
	//	printf("%s \n",tablica[i]);	
}


int main(int argc, char *argv[])
{	
	char** kanalPryw;
//----------------------------------------------------------
//----------------------------------------------------------
  struct sockaddr_un adres;
  
  
  char* kanalRej=malloc(NAME_LEN*sizeof(char));
  int fd,brygFd,bajtCzyt,bajtCzyt2;
	int datagram =0;
	char* otrzymSH1=malloc(CH_SUM*sizeof(otrzymSH1));
	char* tablica=malloc(DL_WIAD*sizeof(char));
	char* wysBajt=malloc(1*sizeof(char));
	char* odebrWiad=malloc(DL_WIAD*sizeof(char));
	char* nazwKan=malloc(DL_NAZWY*sizeof(char));
	int* sfd=malloc(ilRob*sizeof(sfd));
	char* socket_path;
	int ilosc=0;
  if (argc > 1) socket_path=argv[1];

  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&adres, 0, sizeof(adres));
  adres.sun_family = AF_UNIX;

    strncpy(adres.sun_path, socket_path, sizeof(adres.sun_path)-1);
    unlink(socket_path);
  

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
      //continue;
       exit(-1);
    }
	printf("Podaj ID brygady:\n");
    if ( (bajtCzyt=read(brygFd,kanalRej,sizeof(kanalRej))) > 0) {
      printf("Zarejestrowano brygade: %s\n", kanalRej);
      break;
    }
    if (bajtCzyt == -1) {
      perror("read error");
      exit(-1);
    }
    else if (bajtCzyt == 0) {
      printf("EOF\n");
      close(brygFd);
    }
  }
    
    datagram=1;
	
 //--------------------------------------------------------------
 //--------------------------------------------------------------
	
	if(datagram){
	
	char* napis = "viper_serv";
   struct sockaddr_un svaddr[ilRob], claddr[ilRob];
   
   wiadomosc=malloc(ilRob*sizeof(*wiadomosc));
   socklen_t len;
   
   
   for(int i=0;i<ilRob;i++)
   {
   
   kanalPryw= malloc(ilRob*sizeof(*kanalPryw));
   kanalPryw[i] = malloc(sizeof *kanalPryw[i] * DL_NAZWY);
	snprintf(nazwKan,DL_NAZWY,"%s%d",napis,i);
	printf("%s\n",nazwKan);
	kanalPryw[i]=nazwKan;

	
	
   sfd[i] = socket(AF_UNIX, SOCK_DGRAM, 0);       /* Create server socket */
   if (sfd[i] == -1)
      perror("creating socket error ");
			
	

	printf("abstract %s\n",kanalPryw[i]);

   /* Construct well-known address and bind server socket to it */

   if (remove(kanalPryw[i]) == -1 && errno != ENOENT)
        {
	 perror("remove error ");
      exit(-1);
	}

   memset(&svaddr[i], 0, sizeof(struct sockaddr_un));
   svaddr[i].sun_family = AF_UNIX;
   strncpy(&svaddr[i].sun_path[1], kanalPryw[i], strlen(kanalPryw[i]));

   if (bind(sfd[i], (struct sockaddr *) &svaddr[i], 
     sizeof(sa_family_t) + strlen(kanalPryw[i]) + 1) == -1)
       {
			perror("binding error ");
			exit(-1);
		   }


   /* Receive messages, convert to uppercase, and return to client */
   wiadomosc[i]=malloc(sizeof*wiadomosc[i]*DL_WIAD);
}

   while (!done) {
	   
	   
	   for(int i=0;i<ilRob;i++)
	    {
			
			
       len = sizeof(struct sockaddr_un);
       bajtOdebr = recvfrom(sfd[i], wiadomosc[i], DL_WIAD, MSG_DONTWAIT ,
                           (struct sockaddr *) &claddr[i], &len);
        
        //printf("bajty %ld\n",(long int)numBytes);                   
                           
       if (bajtOdebr < 0)
       {	continue;
           perror("recvfrom prob");
	   }
		if(bajtOdebr>0)
		{
		ilosc++;
		otrzymDat = (char**)realloc(otrzymDat, (ilosc+1)*sizeof(*otrzymDat));
		otrzymDat[ilosc-1] = (char*)malloc(40);
		strcpy(otrzymDat[ilosc-1], wiadomosc[i]);
		printf("Server received %s", wiadomosc[i]);		
		}
			int flagi = fcntl(brygFd, F_GETFL, 0);
			fcntl(brygFd, F_SETFL, flagi | O_NONBLOCK);
		if ( (bajtCzyt2=read(brygFd,otrzymSH1,CH_SUM)) > 0)
		{
			printf("suma: %s\n",otrzymSH1);
			
			close(brygFd);
			for(int i=0;i<ilRob;i++)
				{
					
				close(sfd[i]);	
					
				}
					//for(int i=0;i<ilosc;i++)					
			//printf("%s",otrzymDat[i]);	
			
			Sortuj(otrzymDat,ilosc);
					


	for(int i=0;i<ilosc;i++){
		sprintf(tablica,"%c",*otrzymDat[i]);
		sprintf(wysBajt,"%c",tablica[0]);
		strcat(odebrWiad,wysBajt);
	}
	char *sumaSH1=malloc(CH_SUM*sizeof(char));
			sumaSH1=sumaKontr(odebrWiad,strlen(odebrWiad));
	if(strcmp(sumaSH1,otrzymSH1)==0)
		printf("Sumy kontrolne sa zgodne.\n");
	else
		printf("Sumy kontrolne nie są zgodne.\n"
		"Niepelna wiadomosc lub niepoprawna kolejnosc.");
			
			
	printf("Odebrna wiadomosc: %s\n",odebrWiad);
		sleep(2);
		done=1;
		}

		}
		
     }
 }
 //PAMIEC
 	for(int k=0;k<ilRob;k++)
 	{
		free(kanalPryw[k]);
		free(wiadomosc[k]);
	
	}
	for(int k=0;k<=ilosc;k++)
		free(otrzymDat[k]);
	
	//free(kanalPryw);
	//free(wiadomosc);
	free(otrzymDat);
	free(otrzymSH1);
	free(kanalRej);
	free(nazwKan);
	free(sfd);
	free(tablica);
	free(wysBajt);
	free(odebrWiad);
	printf("zwolnil\n");
 
 return 0;
 }
