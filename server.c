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
#define BUF_SIZE 40
#define ilRob 5	
#define SHA_DIGEST_LENGTH 20
	ssize_t numBytes;
	
	//struct sockaddr_un *svaddr, *claddr;
	char **abstract_server;
	char **buf;
	char **array = NULL;
	//

volatile sig_atomic_t done = 0;


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

 
 int porownanie (char **str1, char **str2)
{
  return strcmp(*(str1)+13,*(str2)+13);
}

 
void Sortuj(char ** tablica,int rozmiar)
{
	
	qsort (tablica,rozmiar,sizeof(char *),
                    (int (*)(const void *, const void *)) porownanie);
                    
  // now display the sorted array
	for (int i=0; i < rozmiar; i++) 
		printf("%s \n",tablica[i]);	
}

/*char *str2md5(const char *str, int length) {
//    char *str2md5(const char *str, int length) {


unsigned char hash[SHA_DIGEST_LENGTH];
SHA1((unsigned char*)str, length, hash);
  
   for(int i=0;i<SHA_DIGEST_LENGTH;i++){

        printf("%02x", hash[i]);
    }
  //memset(data, 0, strlen(str));



}*/


int main(int argc, char *argv[])
{	char** abstract_server;
	
	
	/*char datas[5] = "basdt";
size_t length = strlen(datas);
	char *a=malloc(30);
	 a=f1(datas,length);
	 printf("%s",a);*/
//----------------------------------------------------------
//----------------------------------------------------------
  struct sockaddr_un addr;
  
  
  char buf1[100];
  int fd,cl,rc,rc1;
	int start =0;
	char* bufEnd=malloc(128*sizeof(bufEnd));
	char* socket_path;
  if (argc > 1) socket_path=argv[1];

  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    unlink(socket_path);
  

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(fd, 5) == -1) {
    perror("listen error");
    exit(-1);
  }

  while (1) {
    if ( (cl = accept(fd, NULL, NULL)) == -1) {
      perror("accept error");
      continue;
    }
	printf("Podaj ID brygady:\n");
    if ( (rc=read(cl,buf1,sizeof(buf1))) > 0) {
      printf("Zarejestrowano brygade: %s\n", buf1);
      //close(cl);
      //close(fd);
      break;
    }
    if (rc == -1) {
      perror("read");
      exit(-1);
    }
    else if (rc == 0) {
      printf("EOF\n");
      close(cl);
    }
  }


   
    printf("jestem juz po a wiadomosc to %s\n",buf1);
    
    start=1;
	
 //--------------------------------------------------------------
 //--------------------------------------------------------------
	
	if(start){
	char* str=malloc(20);
	char* napis = "viper_serv";
	char* sumak;
   struct sockaddr_un svaddr[10], claddr[10];
   int* sfd=malloc(10*sizeof(sfd));
   buf=malloc(10*sizeof(*buf));
   socklen_t len;
   int count=0;
   
   for(int i=0;i<ilRob;i++)
   {
   
   abstract_server= malloc(10*sizeof(*abstract_server));
   abstract_server[i] = malloc(sizeof *abstract_server[i] * 20);
	snprintf(str,15,"%s%d",napis,i);
	printf("%s\n",str);
	abstract_server[i]=str;
	/*if(i==0)
		strcpy(abstract_server[i],"viper_server2");
	if(i==1)
		strcpy(abstract_server[i],"viper_server21");
	*/
	
	
   sfd[i] = socket(AF_UNIX, SOCK_DGRAM, 0);       /* Create server socket */
   if (sfd[i] == -1)
       printf("socket prob\n");


	printf("abstract %s\n",abstract_server[i]);
//   abstract_server[0] = "viper_server2";
   /* Construct well-known address and bind server socket to it */

   if (remove(abstract_server[i]) == -1 && errno != ENOENT)
       printf("remove prob\n");  

   memset(&svaddr[i], 0, sizeof(struct sockaddr_un));
   svaddr[i].sun_family = AF_UNIX;
   strncpy(&svaddr[i].sun_path[1], abstract_server[i], strlen(abstract_server[i]));

   if (bind(sfd[i], (struct sockaddr *) &svaddr[i], 
     sizeof(sa_family_t) + strlen(abstract_server[i]) + 1) == -1)
       printf("bind prob\n");
   else
		printf("zbindowano");

   /* Receive messages, convert to uppercase, and return to client */
   buf[i]=malloc(sizeof*buf[i]*BUF_SIZE);
}

   while (1) {
	   
	   
	   for(int i=ilRob;i>=0;i--)
	    {
			
			
       len = sizeof(struct sockaddr_un);
       numBytes = recvfrom(sfd[i], buf[i], BUF_SIZE, MSG_DONTWAIT ,
                           (struct sockaddr *) &claddr[i], &len);
        
        //printf("bajty %ld\n",(long int)numBytes);                   
                           
       if (numBytes <= 0)
       {	continue;
           printf("recvfrom prob\n");
	   }
		if(numBytes>0)
		{
		count++;
		array = (char**)realloc(array, (count+1)*sizeof(*array));
		array[count-1] = (char*)malloc(40);
		strcpy(array[count-1], buf[i]);
		printf("Server received %s", buf[i]);		
		}
			int flags = fcntl(cl, F_GETFL, 0);
			fcntl(cl, F_SETFL, flags | O_NONBLOCK);
		if ( (rc1=read(cl,bufEnd,40)) > 0)
		{
			printf("suma: %s\n",bufEnd);
			
			close(cl);
			for(int i=0;i<ilRob;i++)
				{
					
				close(sfd[i]);	
					
				}
					for(int i=0;i<=count;i++)					
			printf("%s",array[i]);	
			
			Sortuj(array,count);
			//char *str2 = malloc(count*sizeof(char));
			//char* c=malloc(1*sizeof(char));
					
		char* c1=malloc(20*sizeof(char));
		char* z=malloc(1*sizeof(char));
		char* cal=malloc(5);

	for(int i=0;i<count;i++){
		sprintf(c1,"%c",*array[i]);
		sprintf(z,"%c",c1[0]);
		strcat(cal,z);
	}
	char *b=malloc(40);
			b=f1(cal,strlen(cal));
	if(strcmp(b,bufEnd)==0)
		printf("SHA DZiALA\n");
			
			
	printf("cal to %s\n",cal);
	
	
	//printf("%s",bufEnd);
	
	//char *output = str2md5(cal, sizeof(cal));
  //      printf("%s\n", output);
	//	printf("%s\n",bufEnd);	
			//printf("MAM STRING %s\n",str2);
			//exit(1);
		}

		
      // for (j = 0; j < numBytes; j++)
      // buf[j] = toupper((unsigned char) buf[j]);

		}
     }
     printf("powinien byc tu\n");
 }
 return 0;
 }
