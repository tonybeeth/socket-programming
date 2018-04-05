#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include"PCommon.h"

int main(int argc, char*argv[]) {
  int socket_fd, port_number = PORT;
  struct sockaddr_in server_address;
  struct hostent *server;

 /* int status;
  struct addrinfo hints, *servinfo;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; //don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; //TCP stream sockets
  //hints.ai_flags = AI_PASSIVE; //fill in my IP (IP of this host) for me

  if(status = getaddrinfo("localhost", NULL, &hints, &servinfo) != 0)
    error("GetAddrInfo error");

  printf("IP addresses\n");
  printAddresses(servinfo);

  freeaddrinfo(servinfo);
*/

  //Open Internet Domain address socket, with stream socket type
  char buffer[256];
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd < 0)
      printError("ERROR opening socket");
  printf("Success opening socket\n");

  //Get Info about server
  if (server = gethostbyname("turing.mwsu.edu")){
    printf("gethostbyname successful\n");
  }else{
      printError("ERROR gethostbyname");
  }

  //Set server address fields
  bzero((char*)&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  memcpy(&server_address.sin_addr.s_addr, server->h_addr, server->h_length); //copy bytes from pointer to server info
  server_address.sin_port = htons(port_number);

//  if(inet_pton(AF_INET, "192.168.127.1", &server_address.sin_addr) <= 0)
  //    error("Invalid Address - Address not supported");

  printf("%lu\n", server_address.sin_addr.s_addr);

  if(connect(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
      printError("ERROR connecting");

  printf("Connecting to server successful\n");

  //Try sending message
  printf("Please enter the message: ");
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);
  int msgLength = write(socket_fd, buffer, strlen(buffer));
  if(msgLength < 0)
      printError("ERROR writing to socket");

  printf("Sending msg to server successful\n");

  //Try reading msg
  bzero(buffer, 256);
  msgLength = read(socket_fd, buffer, 255);
  if(msgLength < 0)
      printError("ERROR reading from socket");

  printf("Reading msg from socket successful\n");

  printf("\nMsg: %s\n", buffer);

  return 0;

}


