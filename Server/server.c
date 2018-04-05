#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include"../common.h"

int main() {
  int socket_fd; //socket file descriptor
  int port_number = PORT; //Use defined port #

  char buffer[256]; //used to store chars from socket connection

  struct sockaddr_in server_address, client_address;

  //Create Internet Domain Address socket, with stream socket type (cont. stream of chars. uses TCP)
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  //Initialize server address to zeros
  bzero((char *) &server_address, sizeof(server_address));

  //Address family for server is Internet Domain Address
  server_address.sin_family = AF_INET;
  //Convert and assign port number from in host byte order to in network byte order (Little indian to Big Indian??)
  server_address.sin_port = htons(port_number);
  //Set IP address of server
  server_address.sin_addr.s_addr = INADDR_ANY;
  printf("%lu ---- %lu \n", INADDR_ANY, server_address.sin_addr.s_addr);

  //Bind socket to address
  if(bind(socket_fd, (struct sock_addr*) &server_address, sizeof(server_address)) < 0) {
    printError("ERROR on binding");
  }

  //Listen for connections (backlog queue max limit of 5)
  listen(socket_fd, 5);
  printf("Now listening . . .\n");

  //Try to establish and accept connection from client
  int client_length = sizeof(client_address);
  int new_socket_fd = accept(socket_fd, (struct sockaddr*) &client_address, &client_length);
  if (new_socket_fd < 0)
    printError("ERROR on accept");

  //Try reading a message from the client and printing it out
  bzero(buffer, 256);
  int msgLength = read(new_socket_fd, buffer, 255);
  if (msgLength < 0)
    printError("ERROR reading from socket");
  printf("Here is the message: %s\n", buffer);

  //Try writing back to client
  msgLength = write(new_socket_fd, "I got your message", 18);
  if(msgLength < 0)
    printError("ERROR on writing to socket");

  return 0;
}
