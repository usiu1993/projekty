#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
//#define BUF_SIZE 100


//int main(int argc, char *argv[])
//{
	/*	int* sfd=malloc(10*sizeof(sfd));
	ssize_t numBytes;
	
	//struct sockaddr_un *svaddr, *claddr;
	char **abstract_server;
	char **buf;
	buf=malloc(10*sizeof(*buf));
	//buf[0]=malloc(sizeof*buf[0]*BUF_SIZE);
	abstract_server= malloc(10*sizeof(*abstract_server));
	abstract_server[0] = malloc(sizeof *abstract_server[0] * 20);
	strcpy(abstract_server[0],"viper_server1");
	strcpy(abstract_server[1],"viper_server2");
	*/
//for(int i=0;i<1;i++)	
//{	
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#define BUF_SIZE 40
	
	ssize_t numBytes;
	
	//struct sockaddr_un *svaddr, *claddr;
	char **abstract_server;
	char **buf;
	char **array = NULL;
	//

volatile sig_atomic_t done = 0;
 
void term(int signum)
{
    done = 1;
}

int main(int argc, char *argv[])
{	char** abstract_server;

	
 
	char* str=malloc(20);
	char* napis = "viper_serv";

   struct sockaddr_un svaddr[10], claddr[10];
   int* sfd=malloc(10*sizeof(sfd));
   buf=malloc(10*sizeof(*buf));
   socklen_t len;
   int count=0;
   
   for(int i=0;i<4;i++)
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
	   
	   
	   for(int i=4;i>=0;i--)
	    {
			
			//abstract_server= malloc(10*sizeof(*abstract_server));
			//abstract_server[i] = malloc(sizeof *abstract_server[i] * 20);
	
			//	if(i==0)
				//	strcpy(abstract_server[i],"viper_server21");
				//if(i==1)
					//strcpy(abstract_server[i],"viper_server2");
			
	   //printf("jest tu %s\n",abstract_server[i]);
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
		
		//for(int i=0;i<count;i++)
		//printf("---->\n%s\n<-------",array[i]);
		
      // for (j = 0; j < numBytes; j++)
      // buf[j] = toupper((unsigned char) buf[j]);

		}
     }
     
     
 }
