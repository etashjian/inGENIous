#include <iostream>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

using std::cout;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
  int sock, length, rc;
  socklen_t fromlen;
  struct sockaddr_in server;
  struct sockaddr_in from;
  char buf[128];

  if (argc < 2)
  {
     fprintf(stderr, "ERROR, no port provided\n");
     exit(0);
  }

  // setup socket
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) error("Opening socket");

  // bind socket
  length = sizeof(server);
  bzero(&server,length);
  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(atoi(argv[1]));
  if (bind(sock,(struct sockaddr *)&server,length)<0) error("binding");

  // wait for packets
  unsigned sequence_num = 0; // start at sequence num 0
  fromlen = sizeof(struct sockaddr_in);
  while (1)
  {
    // wait for next message
    rc = recvfrom(sock,buf,128,0,(struct sockaddr *)&from,&fromlen);
    if (rc < 0) error("recvfrom");
    cout << "Received a datagram: " << buf << "\n";

    // check sequence number
    if(atoi(buf) == sequence_num)
    {
      sequence_num++;
    }

    snprintf(buf, 128, "%d", sequence_num);
    rc = sendto(sock,buf,128,0,(struct sockaddr *)&from,fromlen);
    if (rc < 0) error("sendto");
  }
  return 0;
}
