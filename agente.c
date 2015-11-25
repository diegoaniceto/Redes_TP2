/*
    Simple udp client
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define SERVER "::1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data
 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in6 si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
 
    if ( (s=socket(PF_INET6, SOCK_DGRAM, 0)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin6_family = AF_INET6;
    si_other.sin6_port = htons(PORT);
     
    if (inet_pton(AF_INET6, SERVER , &si_other.sin6_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    
    strcpy(buf, "Mensagem de Teste pra fucionar saporra!");
     
    //send the message
    if (sendto(s, buf, strlen(buf) , 0 , (struct sockaddr *) &si_other, slen) < 0)
    {
        die("sendto()");
    }
     
    //receive a reply and print it
    //clear the buffer by filling null, it might have previously received data
    memset(buf,'\0', BUFLEN);
    //try to receive some data, this is a blocking call
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
    {
        die("recvfrom()");
    }
     
    printf("%s\n", buf);

    close(s);
    return 0;
}