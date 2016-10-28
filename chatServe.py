"""
* chatServe.py
*Author: Joseph Fuerst
* Description: server side of chat program that waits for connections from a client program to chat
* socketing code is adapted from CS 344 code and lectures, and primarily from my Encyption program made in that class as well as the python documentation.
"""

import socket
import sys
import signal
import os


#Handle the interrupt in case the user chooses to close the program with ^c
def signal_handler(signal, frame):
        print('\nExited chatServe.')
        sys.exit(0)
        
signal.signal(signal.SIGINT, signal_handler)


def main():
    
    
    #show user correct usage
    if len(sys.argv) != 3:
        print("Usage: python chatServe.py <hostname> <portno>\n")
        quit()
    
    handle = raw_input("Enter a handle: ") #Get the handle
    s_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)  #set up the socket to accept TCP connections
    s_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    portno = int(sys.argv[2]) #get the port number
    server_address = (socket.gethostbyname(sys.argv[1]), portno) #assign the server address
    s_sock.bind(server_address) #bind the socket to that address
    
    s_sock.listen(1)  #listen for 1 connection
    print "Listening for a connection on " + str(server_address) + "\n"
    
    
    while(1): #loop continuously 
        client, address = s_sock.accept() #accept the connecting socket
        rMessage = client.recv(10) #Get the port number from the connecting client
        print("Now connecting on port " + rMessage + ".\n")
        cntChat = True #loop while both parties want to continue chatting
        while (cntChat == True):
        
            rMessage = client.recv(513) #recieve the message from the user
            
            if rMessage == "\\quit": #check if the user wants to quit.
                cntChat = False;
                print "Client closed connection."
            else:
                print(rMessage)  #otherwise print the message
                
                sMessage = raw_input(handle + "> ") #get a response message
                
                if sMessage == "\\quit": #end chat if server user wants to
                    cntChat = False
                
                else:
                    sMessage = handle + "> " + sMessage  #send the message otherwise
                    
                client.send(sMessage)
        
        client.close() #close the connection when done
        
    return
    
main()