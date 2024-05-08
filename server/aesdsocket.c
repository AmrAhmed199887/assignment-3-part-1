#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> 
#include <stdbool.h>
#include <signal.h>
#include <arpa/inet.h>
bool caught_sigint= false;
bool caught_sigterm= false;

static void signal_handler(int signal_number)
{
    if(signal_number==SIGINT)
    {
        caught_sigint= true;
    }
    if(signal_number== SIGTERM)
    {
        caught_sigterm= true;
    }
}


void * get_in_addr(struct sockaddr* addr)
{

    return &(((struct sockaddr_in *)addr)->sin_addr);
}


int main(int argc, char*argv[])
{


    if(argc == 2)
    {
        if(argv[1][0]=='-' && argv[1][1]=='d')
        {
            printf("starting daemon\n");
            daemon(0,1);
        }
    }

int connect_fd,yes=1;
struct addrinfo info,*addr_sock;
memset(&info,0,sizeof(info));
info.ai_flags = AI_PASSIVE;
info.ai_family=AF_INET;
info.ai_socktype = SOCK_STREAM;

if(getaddrinfo(NULL , "9000" , &info , &addr_sock))
{
    printf("can't getaddrinfo\n");
    openlog(NULL,0,LOG_USER);
    syslog(LOG_ERR,"can't create adress to socket");
    return -1;
}

int socket_FD = socket(addr_sock->ai_family,addr_sock->ai_socktype, 0);
if(socket_FD==-1)
{
        printf("can't create socket\n");
    openlog(NULL,0,LOG_USER);
    syslog(LOG_ERR,"can't create socket");
    close(socket_FD);
    return -1;
}
setsockopt(socket_FD,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes);

switch(fork())
{
    case -1: return -1;
    case 0: break;
    default: _exit(EXIT_SUCCESS);
}
if(bind(socket_FD, addr_sock->ai_addr,addr_sock->ai_addrlen))
{
    printf("can't bind\n");
    openlog(NULL,0,LOG_USER);
    syslog(LOG_ERR,"can't bind socket");
    close(socket_FD);
    return -1;
}

freeaddrinfo(addr_sock);

if(listen(socket_FD,10))
{
    printf("can't listen\n");
    openlog(NULL,0,LOG_USER);
    syslog(LOG_ERR,"listen failed");
    close(socket_FD);
    return -1;
}
FILE *FD;
char ip4[INET_ADDRSTRLEN];
char buffer[100]={0};
int size;
FD=fopen("/var/tmp/aesdsocketdata","a+");
struct sockaddr_storage connect_addr;
socklen_t connect_addr_len;
struct sigaction new_action;
memset(&new_action,0,sizeof(struct sigaction));
new_action.sa_handler=signal_handler;

sigaction(SIGTERM,&new_action,NULL);
sigaction(SIGINT,&new_action,NULL);

while(1)
{
    printf("waiting for connection request\n");
    connect_fd=accept(socket_FD, (struct sockaddr *)&connect_addr,&connect_addr_len);
    if(connect_fd==-1)
    {
        if ( caught_sigint== true || caught_sigterm == true)
    {
        openlog(NULL,0,LOG_USER);
        syslog(LOG_INFO,"Caught signal, exiting");

        break;
    }
        openlog(NULL,0,LOG_USER);
    syslog(LOG_ERR,"accept failed");
    close(socket_FD);
    return -1;
    }
    else
    {

        inet_ntop(connect_addr.ss_family,get_in_addr((struct sockaddr *)&connect_addr),ip4,INET_ADDRSTRLEN);
    printf("accepted connection request from %s\n",ip4);
    openlog(NULL,0,LOG_USER);
    syslog(LOG_INFO,"Accepted connection from %s",ip4);
    size=recv(connect_fd,buffer,100-1, 0);
    char *packet=calloc(size+1,sizeof(char));
    strcpy(packet,buffer);
    fputs(packet,FD);
    free(packet);
    while(buffer[size-1]!='\n')
    {
    memset(buffer,0,sizeof(buffer));
    size=recv(connect_fd,buffer,100-1, 0);
    
    char *packet=calloc(size+1,sizeof(char));
    strcpy(packet,buffer);
    fputs(packet,FD);
    free(packet);
    }
    memset(buffer,0,sizeof(buffer));
    
    
    fseek(FD,0,SEEK_SET);

    while(fgets(buffer,100,FD))
    {
    send(connect_fd,buffer,strlen(buffer),0);
    }

    printf("closed connection from %s\n",ip4);
    openlog(NULL,0,LOG_USER);
    syslog(LOG_INFO,"closed connection from %s",ip4);

    close(connect_fd);

    if ( caught_sigint== true || caught_sigterm == true)
    {
        openlog(NULL,0,LOG_USER);
        syslog(LOG_INFO,"Caught signal, exiting");

        break;
    }
    }

}

if(remove("/var/tmp/aesdsocketdata"))
{
    openlog(NULL,0,LOG_USER);
        syslog(LOG_ERR,"can't remove the file");
}
close(socket_FD);


return 0;
}