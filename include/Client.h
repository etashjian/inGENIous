/**---------------------------------*-c++-*-------------------------------------
 * \file Client.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Header for Client executable for moving streaming project
 *----------------------------------------------------------------------------*/

#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.h"
#include "Common.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <pthread.h>

////////////////////////////////////////////////////////////////////////////////
/**
 * \struct SocketInterface
 * \brief Struct acts as an interface between socket thread and main Client
 */
struct SocketInterface
{
  // Data
  ClientSocket *socket; /// Socket to use
  char *file; /// File to transmit
  std::queue<unsigned> frame_reqs; /// Frame offset of next requested frames

  // Control signals
  unsigned ready = 0; /// Indicates socket is ready for use
  unsigned error = 0; /// Indicates an error occurred in thread
  unsigned file_size = 0; /// size of file on server
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
};

////////////////////////////////////////////////////////////////////////////////
/**
 * \fn bool parse_config_file(char *filename, vector<ClientSocket>& sockets)
 * \brief Parses configuration file with hostnames and ports of all servers 
 *        avilable, and opens sockets for them accordingly
 * \param filename Name of configuration file
 * \param sockets Vector of ClientSockets to initialize
 * \return zero if parse succeeds, non-zero otherwise (error code or something)
 */ 

int parse_config_file(const char *filename, std::vector<ClientSocket>& sockets);

/**
 * \fn int start_servers(vector<SocketInterface>& ifs,vector<pthread_t> threads)
 * \brief Create thread for communication to each server and init servers
 * \param ifs Vector of SocketInterfaces to use indexed by server
 * \param threads Vector of pthreads indexed by server
 * \param sockets ClientSockets for the server threads
 * \param file File for servers to load
 * \return zero if parse succeeds, non-zero on fail
 */
int start_servers(std::vector<SocketInterface>& ifs, 
                  std::vector<pthread_t>& threads,
                  std::vector<ClientSocket>& sockets,
                  char * file);

/**
 * \fn int stream_file(vector<SocketInterface>& ifs, vector<pthread_t> threads)
 * \brief Streams file 
 * \param ifs Vector of SocketInterfaces indexed by server
 * \param threads Vector of pthreads indexed by server
 * \return zero if success, non-zero on fail
 */
int stream_file(std::vector<SocketInterface>& ifs, 
                std::vector<pthread_t>& threads);

/**
 * \fn void* server_thread(void *intf)
 * \brief Producer thread for a given server
 * \param intf Pointer to SocketInterface for this socket
 */
void* server_thread(void *intf);

/**
 * \fn int init_server(const char *file, ClientSocket& socket)
 * \brief Sends init message to server to open and prepare to send file
 * \param i SocketInterface of socket thread calling init
 * \return zero if successful, non-zero on fail
 */
int init_server(SocketInterface *i);

////////////////////////////////////////////////////////////////////////////////
#endif
