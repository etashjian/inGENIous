/**---------------------------------*-c++-*-------------------------------------
 * \file Socket.h
 * \author Edward Tashjian
 * \date October 2015 
 * \brief General purpose UDP sockets
 *----------------------------------------------------------------------------*/
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
/**
 * \class Socket
 * \brief Abstract socket. Defines basic requirements of a socket 
 */
class Socket
{
 public:
  /**
   * \fn Socket()
   * \breif Constructor 
   */
  Socket(int port_) : port(port_) {}

  /**
   * \fn virtual int init()
   * \brief Sets up socket for sending/receiving. Must be called after 
            constructor
   * \return zero if successful, non-zero otherwise
   */
  virtual int init()=0;

  /**
   * \fn virtual bool send()
   * \brief
   */
  virtual int send(char *buffer, size_t buffer_size)=0;

  /**
   * \fn virtual bool receive()
   * \brief
   */
  virtual int receive(char *buffer, size_t buffer_size)=0;

 protected:
  int sock, port;
  socklen_t length;
  struct sockaddr_in server, from;
};

////////////////////////////////////////////////////////////////////////////////
class ClientSocket : public Socket
{
 public:
  ClientSocket(int port_, const char *hostname_) 
    : Socket(port_) 
    , hostname(hostname_)
    {}

  virtual int init();
  
  virtual int send(char *buf, size_t buf_size);

  virtual int receive(char *buf, size_t buf_size);
  
  int configure_timeout(unsigned to_sec, unsigned to_usec); 

 private:
  const char *hostname;
};

////////////////////////////////////////////////////////////////////////////////
class ServerSocket : public Socket
{
 public:
  ServerSocket(int port_) : Socket(port_) {}

  virtual int init();

  virtual int send(char *buf, size_t buf_size);
  
  virtual int receive(char *buf, size_t buf_size);

 private:

};

////////////////////////////////////////////////////////////////////////////////
