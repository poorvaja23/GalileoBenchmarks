#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h> 
#include <arpa/inet.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>

static inline uint64_t RDTSC()
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}
    void error(const char *msg)
     {
      perror(msg);
      exit(0);
     }
//   struct timespec start, end;
struct timeval start, end;
   float uplink, downlink=0.0;
   int main(int argc, char *argv[])
    {
     int sockfd, portno, n;
uint64_t begin=0, finish =0; 
    uint64_t diff1, diff2;
     struct sockaddr_in serv_addr;
     in_addr_t data; 
     struct hostent *server;
     char buffer[256];
     if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
     }
     portno = atoi(argv[2]);
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     server = gethostbyname(argv[1]);
     if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
     }
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    FILE *fp;
    int nread =0;
    /* Receive data in chunks of 256 bytes */
    printf("Receiving \n");
    //begin clocking time to download data from server
//    clock_gettime(CLOCK_MONOTONIC, &start);
gettimeofday(&start, NULL);
//begin = RDTSC(); 
    while((n = read(sockfd, buffer, 256)) > 0)
    {
        nread +=n;
        bzero(buffer, 256); 
    }
  // clock_gettime(CLOCK_MONOTONIC, &end);
gettimeofday(&end, NULL);
//finish = RDTSC();
   diff1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec; 

   if(n < 0)
        printf("\n Read Error \n");
   close(sockfd);
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) ;
       printf("Sending \n");
    fp = fopen("sample.txt","rb");
     if(fp==NULL)
        {
            printf("File open error");
            return 1;   
        }   
     nread =0;
     //begin clocking the time to upload data
  //   clock_gettime(CLOCK_MONOTONIC, &start); 
gettimeofday(&start, NULL);
begin = RDTSC();
        while(1)
        {
            unsigned char buffer[256]={0};
            int nre = fread(buffer,1,256,fp);
            nread += nre; 
            if(nre > 0)
            {
               write(sockfd, buffer, nre);
            }

            if (nre < 256)
            {
                if (feof(fp))
              //      printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }}
    // clock_gettime(CLOCK_MONOTONIC, &end);
gettimeofday(&end, NULL);
finish = RDTSC();
     diff2 = 1000000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec); 
     uplink = (float)(nread*8)/(float)diff2;
     downlink = (float)(nread*8)/(float)diff1;
     printf("downlink = %f bits/sec\n",downlink*1000000000); 
     printf("uplink = %f bits/sec\n",uplink*1000000000);
 printf("uplink time : %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
printf("cycles:");
     printf("%"PRIu64"\n", finish-begin);
     printf("time downlink = %ld\n", diff1);
    printf("%ld\n", diff2);
     close(sockfd);
     return 0;
}
