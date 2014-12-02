/**---------------------------------*-c++-*-------------------------------------
 * \file Socket.cpp
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Definitions for Socket classes for client/server model
 *----------------------------------------------------------------------------*/
#include "Socket.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// ClientSocket Functions
////////////////////////////////////////////////////////////////////////////////
int ClientSocket::init()
{
  // build socket
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
  {
    perror("socket");
    return -1;
  }

  // resolve server hostname
  server.sin_family = AF_INET;
  struct hostent *hp = gethostbyname(hostname);
  if (hp == 0)
  {
    perror("Unknown host");
    return -1;
  }

  // setup rest of server info
  bcopy((char *)hp->h_addr, (char *)&server.sin_addr,hp->h_length);
  server.sin_port = htons(port);
  length = sizeof(struct sockaddr_in);

  int sendbuff;
  socklen_t optlen = sizeof(sendbuff);
  int res = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuff, &optlen);
  cout << sendbuff << endl;
  // return succesfully
  return 0;
}

int ClientSocket::configure_timeout(unsigned to_sec, unsigned to_usec)
{
  struct timeval timeout;
  timeout.tv_sec = to_sec;
  timeout.tv_usec = to_usec;
  if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
  {
    perror("timeout");
    return -1;
  }

  return 0;
}

int ClientSocket::send(char *buf, size_t buf_size)
{
  int rc = sendto(sock,buf,buf_size,0,(const struct sockaddr *)&server,length);
  if (rc < 0)
  {
    perror("Sendto");
    return -1;
  }

  return 0;
}

int ClientSocket::receive(char *buf, size_t buf_size)
{
  int rc = recvfrom(sock,buf,buf_size,0,(struct sockaddr *)&from, &length);
  if (rc < 0)
  {
    //perror("recvfrom");
    return -1;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// ServerSocker Functions
////////////////////////////////////////////////////////////////////////////////
int ServerSocket::init()
{
  // setup socket
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
  {
    perror("Opening socket");
    return -1;
  }

  // bind socket
  length = sizeof(struct sockaddr_in);
  bzero(&server, length);
  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(port);
  if (bind(sock,(struct sockaddr *)&server,length)<0)
  {
    perror("binding");
    return -1;
  }

  // return sucessfully
  return 0;
}

int ServerSocket::send(char *buf, size_t buf_size)
{
  int rc = sendto(sock,buf,buf_size,0,(struct sockaddr *)&from,length);
  if (rc < 0)
  {
    perror("sendto");
    return -1;
  }

  return 0;
}

int ServerSocket::receive(char *buf, size_t buf_size)
{
  int rc = recvfrom(sock,buf,buf_size,0,(struct sockaddr *)&from,&length);
  if (rc < 0)
  {
    perror("recvfrom");
    return -1;
  }

  return 0;
}
