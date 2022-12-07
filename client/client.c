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

// Port for socket
#define PORT   (9000)

// Server address
#define IPV4_ADRESS ("10.0.0.102")

// Flag to indicate interruption
bool interrupted = false;

/**
 * @brief: Handles the received signals
 *
 * @param signo: Integer indicating the signal occurred
 *
 * @return None
 * 
*/
static void signal_handler (int signo)
{
    if(signo == SIGINT || signo == SIGTERM)
    {   
        syslog (LOG_USER,"Exiting client!\n");
        interrupted=true;   
        exit (EXIT_SUCCESS);  
    } 
}

int main()
{
    int sockfd;
    int ret=-1;
    struct sockaddr_in server;
    char packet_received[1024];
    signal (SIGTERM, signal_handler);
    signal (SIGINT, signal_handler);
    
    // Create a socket
    if(((sockfd=socket(PF_INET,SOCK_STREAM,0)))==-1)
    {
        syslog(LOG_USER, "Not able to create socket");
        perror("socket");
        return(-1);
    }

    server.sin_addr.s_addr=inet_addr(IPV4_ADRESS);
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);

    // Connect to server
    while(ret<0)
    {
        ret=connect(sockfd,(struct sockaddr*)&server,sizeof(server));
    }

    while(ret<0)
    {
        ret=connect(sockfd,(struct sockaddr*)&server,sizeof(server));
    }    

    // Keep reading messages
    while(1)
    {
        if(recv(sockfd, packet_received, 1024, 0)<0)
        {
            syslog(LOG_USER, "Receiving failed");
        }
        else
        {
            // Log the incoming message
            syslog(LOG_USER, "Received string");
            syslog(LOG_USER, "Received string is:%s",packet_received);
            
            // Parse the message and print it on the console
            parse(packet_received); 

            memset(packet_received, 0, sizeof(packet_received));

            sleep(1);
        }
    }

    // Close the socket
    close(sockfd);

}
