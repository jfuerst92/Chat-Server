/**************************************************
* chatclient.c
*Author: Joseph Fuerst
* Description: Client side of chat program that connects and chats with a server program.
* socketing code is adapted from CS 344 code and lectures, and primarily from my Encyption program made in that class. 
**************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>	
#include<sys/socket.h>
#include<netinet/in.h>	
#include<netdb.h>


#define rMSize 513
#define sMSize 501



//handle and print error messages, exiting with error
void error(char* msg){
	perror(msg);
	exit(1);
}
//removes the new line character from fgets.
void rmvNewLine(char* buff)
{
   buff[strlen(buff)-1] = '\0';
}

int main(int argc, char *argv[]){
    //check correct arguments input
    if(argc < 2)
    {
		error("Usage: chatclient <hostname> <portno>");
    }
    
    char handle[11]; //holds the name of the typer
    int wVal;
    int rVal;
    int cnt;
    char iBuff[sMSize]; //input and output buffers
    char oBuff[rMSize];
    char* port; //string verstion of portno
    int sockfd;
    int portno;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    
    //Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //create the TCP socket
	if (sockfd < 0) //check that it was created
	{
		error("ERROR: cannot open socket.\n");
		exit(1);
    }
    server = gethostbyname(argv[1]); //get the actual host name
    
    if (server == NULL) //check that it returns a valid server
    {
        fprintf(stderr,"ERROR: host unavailable\n"); 
        exit(0);
    }
    //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(int));
    bzero((char *) &serv_addr, sizeof(serv_addr)); //zero server address
    portno = atoi(argv[2]); //get port number 
    serv_addr.sin_family = AF_INET; //set up serv_addr to handle communications
    serv_addr.sin_port = htons(portno);
    
    bcopy((char*)server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    
    
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) //connect
	{
		error("ERROR: cannot connect to port\n");
		exit(1);
    }
    
    printf("\nEnter Handle: "); ///prompt the user to enter a handle
    fgets(handle, sizeof(handle), stdin);
    handle[strlen(handle)-1] = '\0';
    port = argv[2]; //get the string port
    
    wVal = write(sockfd,port,strlen(port)); //write the port number to the server
    if (wVal < 0) 
    {
		error("ERROR: write to socket failed.\n");	
    }
    cnt = 1;
    while(cnt == 1) //chat continues unti a user enters \quit
    {
        printf("%s> ", handle); //get user message
        bzero(iBuff, sMSize);
        fgets(iBuff, sMSize-1, stdin);
        rmvNewLine(iBuff);
        
        if(strcmp(iBuff, "\\quit") == 0)  //see if the user wants to quit
        {//checking if the client wants to quit
			cnt = 0;
			wVal = write(sockfd,iBuff,strlen(iBuff));//telling the server the client's quitting
			if (wVal < 0) 
            {
				error("ERROR: write to socket failed.\n");
			}
        }
        else //continue chatting
        {
            bzero(oBuff, rMSize); //set up the output buffer to send the message
            strcpy(oBuff, handle); //append the handle and > to the message
            strcat(oBuff, "> ");
            strcat(oBuff, iBuff);
             
            wVal = write(sockfd,oBuff,strlen(oBuff)); //write the message and send
            if (wVal < 0) 
            {
				error("ERROR: write to socket failed.\n");
			}
            
            bzero(oBuff, rMSize);
            
            rVal = read(sockfd, oBuff, rMSize); //read the response
            if (rVal < 0) 
            {
				error("ERROR: read from socket failed.\n");
			}
            else if (strcmp(oBuff, "\\quit") == 0) //if server quit
            {
                cnt = 0;
            }
            else if (rVal == 0)
            {
                cnt = 0;
            }
            else
            {
                printf("%s\n", oBuff); //print the response
            }
        }
        
    }
    
    close(sockfd); //the chat has ended
    
    return 0;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}