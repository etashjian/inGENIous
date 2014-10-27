/**---------------------------------*-c++-*-------------------------------------
 * \file Socket.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015 
 * \brief General purpose UDP sockets
 *----------------------------------------------------------------------------*/

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
/**
 * \class Socket
 * \brief Abstract socket. Defines basic requirements of a socket 
 */
class Socket
{
 public:
  /**
   * \fn Socket(int port_)
   * \brief Socket constructor
   * \param port Port number of server 
   */
  Socket(int port_) : port(port_) {}

  /**
   * \fn virtual int init()
   * \brief Sets up socket for sending/receiving. Must be called after 
   *        constructor
   * \return zero if successful, non-zero otherwise
   */
  virtual int init()=0;

  /**
   * \fn virtual bool send(char *buffer, size_t buffer_size)
   * \brief Sends specified number of bytes from character array to target
   * \param buffer Character array of send data
   * \param buffer_size Size of information to send from buffer
   * \return Zero if successful, non-zero if failure
   */
  virtual int send(char *buffer, size_t buffer_size)=0;

  /**
   * \fn virtual bool receive(char *buffer, size_t buffer_size)
   * \brief Receives specified number of bytes into buffer from socket
   * \param buffer Character array to store received data
   * \param buffer_size Size of data to receive
   * \return Zero if successful, non-zero if failure
   */
  virtual int receive(char *buffer, size_t buffer_size)=0;

 protected:
  int sock, /// Socket identifier
      port; /// Port number of server socket connects to
  socklen_t length; /// Convenience variable for holding struct lengths
  struct sockaddr_in server, from; /// Actual sockets
};

////////////////////////////////////////////////////////////////////////////////
/**
 * \class ClientSocket
 * \brief Socket type to be used on Client executable. Can communicate to a 
 *        ServerSocket on a server machine specified in ClientSocket constructor
 */
class ClientSocket : public Socket
{
 public:
  /**
   * \fn ClientSocket(int port_, const char *hostname_) 
   * \brief ClientSocket Constructor
   * \param port_ Port number of server
   * \param hostname_ Hostname of server
   */
  ClientSocket(int port_, const char *hostname_) 
    : Socket(port_) 
    , hostname(hostname_)
    {}

  /**
   * \fn virtual int init()
   * \brief Sets up ClientSocket for sending/receiving. Must be called after 
   *        constructor
   * \return zero if successful, non-zero otherwise
   */
  virtual int init();
   
  /**
   * \fn virtual bool send(char *buffer, size_t buffer_size)
   * \brief Sends specified number of bytes from character array to server
   * \param buffer Character array of send data
   * \param buffer_size Size of information to send from buffer
   * \return Zero if successful, non-zero if failure
   */
  virtual int send(char *buf, size_t buf_size);

  /**
   * \fn virtual bool receive(char *buffer, size_t buffer_size)
   * \brief Receives specified number of bytes into buffer from server
   * \param buffer Character array to store received data
   * \param buffer_size Size of data to receive
   * \return Zero if successful, non-zero if failure
   */
  virtual int receive(char *buf, size_t buf_size);
  
  /**
   * \fn int configure_timeout(unsigned to_sec, unsigned to_usec)
   * \brief Set up a timeout on receive function of ClientSocket
   * \param to_sec Number of seconds in timeout
   * \param to_usec NUmber of microseconds in timeout
   * \return Zero if successful, non-zero if failure
   */
  int configure_timeout(unsigned to_sec, unsigned to_usec); 

 private:
  const char *hostname; /// Hostname of target server
};

////////////////////////////////////////////////////////////////////////////////
/**
 * \class ServerSocket
 * \brief Socket type to be used on Server executable. Can connect to a 
 *        ClientSocket to send/receive data
 */
class ServerSocket : public Socket
{
 public:
  /**
   * \fn ServerSocket(int port_)
   * \brief ServerSocket Constructor
   * \param port_ Port number of server application
   */
  ServerSocket(int port_) : Socket(port_) {}
  
   /**
   * \fn virtual int init()
   * \brief Sets up ClientSocket for sending/receiving. Must be called after 
   *        constructor
   * \return zero if successful, non-zero otherwise
   */
  virtual int init();

  /**
   * \fn virtual bool send(char *buffer, size_t buffer_size)
   * \brief Sends specified number of bytes from character array to client
   * \param buffer Character array of send data
   * \param buffer_size Size of information to send from buffer
   * \return Zero if successful, non-zero if failure
   */
  virtual int send(char *buf, size_t buf_size);
  
  /**
   * \fn virtual bool receive(char *buffer, size_t buffer_size)
   * \brief Receives specified number of bytes into buffer from client
   * \param buffer Character array to store received data
   * \param buffer_size Size of data to receive
   * \return Zero if successful, non-zero if failure
   */
  virtual int receive(char *buf, size_t buf_size);

};

////////////////////////////////////////////////////////////////////////////////
#endif
