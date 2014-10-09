#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

using std::cout;

void error(const char *);
int main(int argc, char *argv[])
{
  int sock, n;
  unsigned int length;
  struct sockaddr_in server, from;
  struct hostent *hp;
  char buffer[128];

  if (argc != 3)
  {
    cout << "Usage: cleint hostaname port\n";
    exit(1);
  }

  // build socket
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) error("socket");

  // set timeout to 2 s
  struct timeval timeout;
  timeout.tv_sec = 2;
  timeout.tv_usec = 0;
  if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    error("timeout");

  server.sin_family = AF_INET;
  hp = gethostbyname(argv[1]);
  if (hp==0) error("Unknown host");

  bcopy((char *)hp->h_addr, (char *)&server.sin_addr,hp->h_length);
  server.sin_port = htons(atoi(argv[2]));
  length=sizeof(struct sockaddr_in);

  unsigned sequence_num = 0;
  while(1)
  {
    bool timeout = false;
    do
    {
      if(timeout) cout << "TIMEOUT\n";
      timeout = true;

      cout << "Sending sequence number: " << sequence_num << "\n";

      bzero(buffer,128);
      snprintf(buffer, 128, "%d", sequence_num);
      n=sendto(sock,buffer,128,0,(const struct sockaddr *)&server,length);
      if (n < 0) error("Sendto");

    } while(recvfrom(sock,buffer,128,0,(struct sockaddr *)&from, &length) < 0);
    //sleep(5);
    cout << "Receiver waiting for: " << buffer << "\n";
    if(sequence_num < atoi(buffer)) sequence_num++;
  }

  close(sock);
  return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
