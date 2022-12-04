#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include<stdio.h>
#include"parser.h"

#define PORT   (9000)
#define IPV4_ADRESS ("192.168.0.2")
bool interrupted=false;

static void signal_handler (int signo)
{
    if(signo == SIGINT || signo == SIGTERM){   
    syslog (LOG_USER,"Exiting client!\n");
    interrupted=true;   
    exit (EXIT_SUCCESS);  
    } 
}

int main(){
int sockfd;
int ret=-1;
struct sockaddr_in server;
//char* packet_recieved=(char*)malloc(1024*sizeof(char));
char packet_recieved[1024];
signal (SIGTERM, signal_handler);
signal (SIGINT, signal_handler);

openlog("aesd-client",LOG_PID|LOG_ERR,LOG_USER); 
setlogmask(LOG_UPTO(LOG_DEBUG)); 
    if(((sockfd=socket(PF_INET,SOCK_STREAM,0)))==-1){
    syslog(LOG_USER, "Not able to create socket");
    perror("socket");
    return(-1);
}
server.sin_addr.s_addr=inet_addr(IPV4_ADRESS);
server.sin_family=AF_INET;
server.sin_port=htons(PORT);
while(ret<0){
    ret=connect(sockfd,(struct sockaddr*)&server,sizeof(server));
}
//while(!interrupted){ 
while(ret<0){
    ret=connect(sockfd,(struct sockaddr*)&server,sizeof(server));
}    
if(recv(sockfd,packet_recieved,1024,0)<0){
     syslog(LOG_USER, "Reciving failed");
}
else{
syslog(LOG_USER, "Recieved string");
syslog(LOG_USER, "Recieved string is:%s",packet_recieved);
parse(packet_recieved);
memset(packet_recieved,0,sizeof(packet_recieved));
}
//}
close(sockfd);
}