#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

//to display error messages
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, last;
     socklen_t clilen;
     char buffer[256]; //buffer size is set to 256 bytes
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, port not mentioned\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0); 
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY; 
     serv_addr.sin_port = htons(portno);

     last = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
     if(last <0) 
          error("ERROR on binding");

     listen(sockfd,5);

     clilen = sizeof(cli_addr);
    //wait for connection from client
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     bzero(buffer,256);
        FILE *fp = fopen("sample.txt","rb");// file to transfer
        if(fp==NULL)
        {
            printf("error in opening file");
            return 1;   
        }   
     int nread =0;
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buffer[256]={0};
            int nre = fread(buffer,1,256,fp);
            nread += nre; 
            if(nre > 0)
            {
                printf("Sending \n");
                write(newsockfd, buffer, nre);
            }
            if (nre < 256)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading the file\n");
                break;
            }}
    close(newsockfd);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
    
    bzero(buffer, 256);
  
    nread=0;
    while((n = read(newsockfd, buffer, 256)) > 0)
     {
        nread +=n;
        printf("buffer = %s", buffer);
        printf("Receiving \n");
        bzero(buffer, 256);    
     }
   close(newsockfd);
   close(sockfd);     
     return 0; 
}
