/* a server in the unix domain.  The pathname of 
   the socket address is passed as an argument */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <signal.h>

/*void *SigCatcher(int n)
{
  wait3(NULL,WNOHANG,NULL);
}*/
void error(const char *);
void dostuff(int); /* function prototype */

   int pipefd[2];
       //pid_t cpid;
       char buf;

      

int main(int argc, char *argv[])
{
   int sockfd, newsockfd, servlen, pid;
   socklen_t clilen;
   struct sockaddr_un  cli_addr, serv_addr;
   char bufi[80];
   //--------------
     pipe(pipefd);
//--------------
   if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
       error("creating socket");
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sun_family = AF_UNIX;
   strcpy(serv_addr.sun_path, argv[1]);
   servlen=strlen(serv_addr.sun_path) + 
                     sizeof(serv_addr.sun_family);
   if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0)
       error("binding socket"); 

   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   //---------
   //signal(SIGCHLD,SigCatcher);
   signal(SIGCHLD,SIG_IGN);
   //-------------
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
			//-------------------- 
			//  close(pipefd[1]); 
			   // while (read(pipefd[0], &bufi, 1) > 0) // read while EOF
               //write(1, &bufi, 1);
           //write(1, "\n", 1);
          // close(pipefd[0]);
           //-----------
			  printf("jak na razie bufi to: %s\n",bufi);
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else {
			
			 
			 close(newsockfd);
			 }
     } 
   
   close(sockfd);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void dostuff (int sock)
{
   int n;
   char buffer[256];
   //char extrabuff[500];
      
   bzero(buffer,256);
   n = read(sock,buffer,255);
   //----------
  //------------
		//	 close(pipefd[0]); // close the read-end of the pipe, I'm not going to use it
        //   write(pipefd[1], buffer, strlen(buffer)); // send the content of argv[1] to the reader
        //   close(pipefd[1]);
           //----------------
   //----------
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
   //printf("and whole mess: %s\n",extrabuff);
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}
