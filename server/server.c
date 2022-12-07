#include<sys/types.h>
#include<sys/socket.h>
#include<sys/syslog.h>
#include<errno.h>
#include<signal.h>
#include<sys/syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include<stdio.h>
#include <netdb.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <string.h>

// Sensor headers
#include "../gps_application_code/gps.h"
#include "../mma8452q_application_code/mma8452.h"

#define PORT "9000"
#define BACKLOG (5)
#define BUFFER_SIZE (1024)
struct addrinfo *server_info=NULL;
bool interrupted=false;

// Sensor params
#define SENSOR_DATA_LENGTH 512
int gpsFd = -1;
int acclFd = -1;

static void signal_handler (int signo)
{
    if(signo == SIGINT || signo == SIGTERM){   
    syslog (LOG_USER,"Exiting Server!\n");
    interrupted=true;   
    exit (EXIT_SUCCESS);  
    } 
}

int main(int argc,char* argv[]){
int tr=1;   
char clientIP[INET6_ADDRSTRLEN]; 
char* message;

signal (SIGTERM, signal_handler);
signal (SIGINT, signal_handler);

openlog("aesd-socket",LOG_PID|LOG_ERR,LOG_USER); 
setlogmask(LOG_UPTO(LOG_DEBUG));  
struct sockaddr client_addr;
struct addrinfo hints;
socklen_t address_len=sizeof(struct sockaddr);
socklen_t addr_size=sizeof(client_addr);
int sockfd,new_sockfd,s_send;

//message="4000.81269 N 0515.92189 W 234942.00";

// Open sensor ports
gpsFd = OpenPort();
acclFd = open_i2c_port();

if(((sockfd=socket(PF_INET,SOCK_STREAM,0)))==-1){
    syslog(LOG_USER, "Not able to create socket");
    perror("socket");
    return(-1);
}
if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
    perror("setsockopt");
    return(-1);
}
memset(&hints,0,sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;
hints.ai_protocol=0;

if(getaddrinfo(NULL,PORT,&hints,&server_info)!=0){
    syslog(LOG_USER, "Error while getting address");
    return(-1);
}

if(bind(sockfd,server_info->ai_addr,sizeof(struct sockaddr))<0){
    syslog(LOG_USER, "Binding not done.");
    perror("bind");
    return(-1);
}

freeaddrinfo(server_info);
syslog(LOG_USER, "Listening");
listen(sockfd,BACKLOG);

new_sockfd=accept(sockfd,&client_addr,&addr_size);

if(new_sockfd==-1)
{
    syslog(LOG_USER, "Error while accepting");
    perror("accept");
    close(sockfd);
    return(-1);
}

syslog(LOG_USER, "Accepted Connection");
inet_ntop(AF_INET,&client_addr,clientIP,sizeof(clientIP));


while(!interrupted)
{ 
    char sensorData[SENSOR_DATA_LENGTH];
    char gpsData[100];
    char acclData[100];
    
    // Populate the sensor data
    int gpsBytes = PopulateGpsData(gpsFd, sensorData, SENSOR_DATA_LENGTH);
    sensorData[gpsBytes] = ' ';

    strncpy(gpsData, sensorData, gpsBytes);
    gpsData[gpsBytes] = '\0';
    printf("GPS data: %s\n", gpsData);
    printf("GPS bytes: %d\n\n", gpsBytes);

    int acclBytes = populate_accl_data(sensorData + gpsBytes + 1, SENSOR_DATA_LENGTH - gpsBytes - 1, acclFd);

    strncpy(acclData, sensorData + gpsBytes + 1, acclBytes);
    acclData[gpsBytes + acclBytes + 1] = '\0';
    printf("Accl Data: %s\n", acclData);
    printf("Accl bytes: %d\n\n", acclBytes);

    //sensorData[gpsBytes + acclBytes + 1] = ' ';
    sensorData[gpsBytes + acclBytes + 1] = '\0';
    printf("Sensor Data: %s\n", sensorData);

    if((s_send=send(new_sockfd, sensorData, (strlen(sensorData) * sizeof(char)),0))<0)
    {
        syslog(LOG_USER, "Sending failed"); 
    }
      
} 
close(sockfd);          
}
