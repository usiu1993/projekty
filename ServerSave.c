#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#define MAX 10000
int main(int argc, char** argv)
{
	char buf[200];
	int ntime;
	int opt;
	int server_to_client[MAX];
	int num;
	
	if(argc!=4)
	{
		printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                exit(1);

	}
        while ((opt = getopt(argc, argv, "s:t:d:")) != -1) 
	{
               switch (opt) 
	       {
               case 's':
                   strcpy(buf,optarg);
                   break;
               case 't':
                   ntime = atoi(optarg);
                   break;
				case 'd':
                   num = atoi(optarg);
                  // printf("num w serwerze to %d",num);	
                   break;
               default:
                   printf("Usage: %s [-t ntime(sec)] [-c string]\n", argv[0]);
                   exit(1);
               }
        }
	
	struct timespec spi;
		spi.tv_sec=ntime;
		spi.tv_nsec=0;

	
	
	
   char *myfifo2 = "/tmp/new";

  
	//printf("jestem tu a num to %d\n",num);
   mkfifo(myfifo2, 0666);
	
   server_to_client[num] = open(myfifo2, O_WRONLY| O_CLOEXEC);

   printf("Server ON.\n");

         printf("Sending...\n");
         write(server_to_client[num],buf,BUFSIZ);
    
    memset(buf, 0, sizeof(buf));
	nanosleep(&spi,NULL);
   close(server_to_client[num]);
   
   unlink(myfifo2);
   return 0;
}
