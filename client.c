#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include"common.h"

int main(int argc, char*argv[]) {
  int socket_fd, port_number = PORT;
  struct sockaddr_in server_address;
  struct hostent *server;

  //Open Internet Domain address socket, with stream socket type
  char buffer[256];
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd < 0)
    error("ERROR opening socket");
  printf("Success opening socket\n");

  //Get Info about server
  if (server = gethostbyname("localhost")){
    printf("gethostbyname successful\n");
  }else{
    error("ERROR gethostbyname");
  }

  //Set server address fields
  bzero((char*)&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  bcopy((char*)&server->h_addr, (char*)&server_address.sin_addr.s_addr, server->h_length); //copy bytes from pointer to server info
  server_address.sin_port = htons(port_number);

  if(connect(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
    error("ERROR connecting");

  printf("Connecting to server successful\n");

  //Try sending message
  printf("Please enter the message: ");
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);
  int msgLength = write(socket_fd, buffer, strlen(buffer));
  if(msgLength < 0)
    error("ERROR writing to socket");

  printf("Sending msg to server successful\n");

  //Try reading msg
  bzero(buffer, 256);
  msgLength = read(socket_fd, buffer, 255);
  if(msgLength < 0)
    error("ERROR reading from socket");

  printf("Reading msg from socket successful\n");

  printf("\nMsg: %s\n", buffer);

  return 0;

}


