/*
    Simple udp server
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in6 si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN], messageRecv[BUFLEN];
     
    //create a UDP socket
    if ((s=socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin6_family = AF_INET6;
    si_me.sin6_port = htons(PORT);
    si_me.sin6_addr = in6addr_any;
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
        strcpy(messageRecv, buf); 
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntop(AF_INET6, 
        	&(si_other.sin6_addr), buf, sizeof(buf)), ntohs(si_other.sin6_port));
        printf("Data: %s\n" , messageRecv);
         
        //now reply the client with the same data
        if (sendto(s, messageRecv, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
 
    close(s);
    return 0;
}