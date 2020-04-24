/*#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "irc.h"

struct arg_struct {
  int sock;
};

char buffer[BUFFER_SIZE];

void* receiveMsg(void* arguments) {
  struct arg_struct args = *((struct arg_struct*)arguments);
  int len;
  // Client thread always ready to receive message
  while ((len = recv(args.sock, buffer, BUFFER_SIZE, 0)) > 0) {
    buffer[len] = '\0';
    printf("%s\n", buffer);
  }
}

int main(int argc, char const* argv[]) {
  pthread_t recvt;
  int len, sock;
  char sendMessage[BUFFER_SIZE];
  struct sockaddr_in serverIp;
  struct hostent* server;
  if (argc < 3) {
    error("Usage Error!\nCorrect use is: ./clientTest server-port your-name");
  }
  char clientName[100];
  strcpy(clientName, argv[2]);
  sock = socket(AF_INET, SOCK_STREAM, 0);
  serverIp.sin_port = htons(atoi(argv[1]));
  serverIp.sin_family = AF_INET;
  serverIp.sin_addr.s_addr = inet_addr("127.0.0.1");

  if ((connect(sock, (struct sockaddr*)&serverIp, sizeof(serverIp))) == -1) {
    error("\nConnection to socket falied\n");
  }
  memset(sendMessage, 0, BUFFER_SIZE);

  // creating a client thread which is always waiting for a message
  struct arg_struct args;
  args.sock = sock;

  pthread_create(&recvt, NULL, (void*)receiveMsg, &args);

  while (fgets(buffer, BUFFER_SIZE, stdin) > 0) {
    strcpy(sendMessage, clientName);
    strcat(sendMessage, ":");
    strcat(sendMessage, buffer);
    len = write(sock, sendMessage, strlen(sendMessage));
    if (len < 0) printf("\n message not sent \n");
  }

  // thread is closed
  pthread_join(recvt, NULL);
  close(sock);

  return 0;
}*/


#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>

void *recvmg(void *my_sock) {
  int sock = *((int *)my_sock);
  int len;
  char msg[4096];
  // client thread always ready to receive message
  while ((len = recv(sock, msg, 4096, 0)) > 0) {
    msg[len] = '\0';
    fputs(msg, stdout);
    //std::cout << msg;
  }
}

int main(int argc, char *argv[]) {
  pthread_t recvt;
  int len;
  int sock;
  struct sockaddr_in ServerIp;
  std::string client_name;

  client_name = argv[1];
  //strcpy(client_name, argv[1]);
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  
  ServerIp.sin_port = htons(atoi(argv[2]));
  ServerIp.sin_family = AF_INET;
  ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  if ((connect(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp))) == -1){
    perror("connect: ");
    exit(EXIT_FAILURE);
  }

  // creating a client thread which is always waiting for a message
  pthread_create(&recvt, NULL, recvmg, &sock);

  // ready to read a message from console
  char msg[4096];
  std::string send_msg;
  while (fgets(msg, 4096, stdin) > 0) {
    send_msg = client_name + ": " + msg;
    len = write(sock, send_msg.c_str(), send_msg.length());
    if (len < 0){
      std::cout << "\nWarning: Message not sent!\n";
    }
  }

  // thread is closed
  pthread_join(recvt, NULL);
  close(sock);
  return 0;
}
