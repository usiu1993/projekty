#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define DANE 31
#define NAZWA 30
#define errorExit(blad) { fprintf(stderr,blad); fprintf(stderr, "nr bledu: %d\n", errno); exit(EXIT_FAILURE); } 


 void sigpipe_handler(int signum){

        printf("Otrzymano sygnał SIGPIPE %d\n",signum);
        
}

void remove_element(int *array, int index, int array_length)
{
   int i;
   for(i = index; i < array_length - 1; i++) array[i] = array[i + 1];
}


void zamykanie(int* kolejka, int iFifo)
{
	
						for(int i=0;i<iFifo;i++)
						{
						if(kolejka[i]==0)
						{
						for(int j=i;j<iFifo;j++)
						kolejka[j]=kolejka[j+1];
						iFifo--;
						if(iFifo==0)
								errorExit("wszystkie kanały nie są aktywne\n");
						}
						}
								
	
	}


void odczytZapis(int* kolejka,int iFifo)
{
	
	
	struct sigaction sigpipeS;
	sigpipeS.sa_handler = sigpipe_handler;
    sigpipeS.sa_flags   = 0;    

    sigemptyset( &sigpipeS.sa_mask); 
    sigaddset  ( &sigpipeS.sa_mask, SIGINT);

	 ssize_t ilBajt;
	 
    char buffer[DANE];
    for(;;) {

		while(read(STDIN_FILENO, buffer, 32)){
	
       for(int i=0;i<iFifo;i++){
		//int FifoRem=iFifo;
			if(kolejka[i]!=0){
				//nieaktywne--;
				//printf(" w kol %d\n",nieaktywne);
					if (sigaction(SIGPIPE, &sigpipeS, NULL) == -1)
		{
				errorExit("sigaction error\n");
		}
				
				if ((ilBajt = write(kolejka[i], buffer, DANE)) == -1){
				//int abc;
					   fprintf(stderr,"blad odczytu, zamkniety strumien, blad nr %d\n",errno);
                       kolejka[i]=-1;
					   close(kolejka[i]);
                       //unlink(kolejka[i]);
                       fprintf(stderr,"wyrzucam kanał /tmp/%d\n",i);
        
						for(int i=0;i<iFifo;i++)
						{
						if(kolejka[i]==-1)
						{
						
							
						for(int j=i;j<iFifo;j++)
						kolejka[j]=kolejka[j+1];
						iFifo--;
						close(kolejka[i]);
						if(iFifo==0)
						{	
								//close(kolejka[k]);
								errorExit("wszystkie kanały nie są aktywne\n");//Decreasing the length of the array
						}
						}
						
						}
						
				}
				else if(ilBajt<DANE)
				{
					fprintf(stderr,"blad odczytu, przepelnione fifo\n");
					kolejka[i]=-1;
					close(kolejka[i]);
					fprintf(stderr,"wyrzucam kanał /tmp/%d\n",i); 
					for(int i=0;i<iFifo;i++)
						{
						if(kolejka[i]==-1)
						{
						for(int k=i;k<iFifo;k++)
						kolejka[k]=kolejka[k+1];
						iFifo--;
						if(iFifo==0)
						{
							
								errorExit("wszystkie kanały nie są aktywne\n");//Decreasing the length of the array
						}
						}
						}
				
				}
			/*	else
				printf("przeslano %ld bajtow\n", ilBajt);*/
				}
			
			}}
			
			
			 break;
		
}
     
              
    }


int main(int argc, char** argv)
{	
	
	int DIAGNOSTIC =0;	
	int opt;
	int ilKolejek;
	//wiad=(char*) malloc(BLOCK*sizeof(*wiad));
	char* nazwa=(char*) malloc(20*sizeof(*nazwa));
	char* nr=(char*) malloc(3*sizeof(*nr));
	char* nazwaPliku=(char*) malloc(20*sizeof(*nazwaPliku));
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

  for (int i = 0 ; i<ilKolejek; i++)
  {
	struct stat sBuf;
    kolejki[i] = malloc(sizeof *kolejki[i] * NAZWA);
    sprintf(nr,"%d",i);
	strcat(nazwaKol, nr);

		memset (&sBuf, 0, sizeof (sBuf));
	stat(nazwaKol, &sBuf); 

	if (S_ISFIFO(sBuf.st_mode))
	{	//printf("w fifo %d\n",i);
		strcpy(kolejki[i],nazwaKol);
	}
	else
		fprintf(stderr,"%s nie jest plikiem fifo\n",nazwaKol);
		    
    strcpy(nazwaKol, nazwa);
 
  }
	int *d=(int*) malloc(ilKolejek*sizeof(*d));
	for(int j=0;j<ilKolejek;j++)
	{
		
		printf("%s\n",kolejki[j]);

		printf("------------\n");
		}
	
	
	
	int ile=0;
	int* aktywneKol=(int*)malloc(1*sizeof(*aktywneKol));
	for(int k=0;k<ilKolejek;k++)
	{	
		fd[k]=open(kolejki[k],O_WRONLY | O_NONBLOCK);	
//fd[k]=open(kolejki[k],O_RDWR | O_NONBLOCK);
		if(fd[k]!=-1)
		{ile++;
			aktywneKol=realloc(aktywneKol,ile*sizeof(*aktywneKol));
			//printf("%s is opened to write\n",kolejki[k]);
			//d[k]=fd[k];	
			aktywneKol[ile-1]=fd[k];
		}
		else if(strcmp(kolejki[k],"")!=0){
			//d[k]=0;
		fprintf(stderr, "Blad otwarcia fifo %s, nr bledu %d\n",kolejki[k],errno);
		close(fd[k]);
	}		
	}
	
	
	for(int a=0;a<ile;a++)
		printf("dd %d\n",aktywneKol[a]);
	
	printf("ILE TO %d\n",ile);
	odczytZapis(aktywneKol,ile);
	
	if(DIAGNOSTIC)
{		
	//fflush(stdout);
	 close(diag);
}	
	fprintf(stderr,"\n===========================\n");
	fprintf(stderr,"Koniec dzialania programu. \n");
	fprintf(stderr,"===========================\n");
	/*
	for(int i=0;i<ile;i++)
	{
		close(dd[i]);
	}
		*/
	//free(dd);
	//free(fd);

	//free(nr);
	//free(nazwaKol);
	//free(nazwaPliku);
	//free(nazwa);

	/*
	for(int i=0;i<ilKolejek;i++)
	{
		int ret = remove(kolejki[i]);
		if (ret != 0)  {
			fprintf(stderr,"\nnie usunieto %s",kolejki[i]);
			fprintf(stderr,"\nerrno: %d",errno);
			exit(9);
		}
		unlink(kolejki[i]);
		free(kolejki[i]);
		
		}
	*/
	return 0;

}
