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
#include <list>
#include <set>
#include <unordered_set>
#include <pthread.h>
#include <sys/time.h>
#include <sstream>
#include <getopt.h>

////////////////////////////////////////////////////////////////////////////////
struct timeval start_time; /// Reference time
unsigned init_window_size = DEFAULT_INIT_WINDOW_SIZE;
unsigned init_queue_size = DEFAULT_MAX_QUEUE_SIZE;
unsigned num_frames = 0;

//std::queue<unsigned> index_queue;
pthread_mutex_t deque_lock = PTHREAD_MUTEX_INITIALIZER;
std::deque<unsigned> index_deque;

////////////////////////////////////////////////////////////////////////////////
/**
 * \struct SocketInterface
 * \brief Struct acts as an interface between socket thread and main Client
 */
struct SocketInterface
{
  // Data
  ClientSocket *socket; /// Socket to use
  std::queue<unsigned> frame_reqs; /// Frame offset of next requested frames

  // Control signals
  unsigned id = 0; /// Id of thread
  unsigned ready = 0; /// Indicates socket is ready for use
  unsigned error = 0; /// Indicates an error occurred in thread
  unsigned queue_size = 0; 
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t full = PTHREAD_COND_INITIALIZER;
  pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
};

////////////////////////////////////////////////////////////////////////////////
/**
 * \fn int parse_cmdline(int argc, char **argv)
 * \brief Parses and handles command line arguments
 * \param argc Program argc input
 * \param argv Program argv input
 * \return zero if successful, non-zero otherwise
 */
int parse_cmdline(int argc, char **argv);

/**
 * \fn int parse_config_file(char *filename, vector<ClientSocket>& sockets)
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
 * \return zero if parse succeeds, non-zero on fail
 */
int start_servers(std::vector<SocketInterface>& ifs, 
                  std::vector<pthread_t>& threads,
                  std::vector<ClientSocket>& sockets);

/**
 * \fn int stream_data(vector<SocketInterface>& ifs, vector<pthread_t> threads)
 * \brief Streams file 
 * \param ifs Vector of SocketInterfaces indexed by server
 * \param threads Vector of pthreads indexed by server
 * \return zero if success, non-zero on fail
 */
int stream_data(std::vector<SocketInterface>& ifs, 
                std::vector<pthread_t>& threads);
int stream_data_non_blocking(std::vector<SocketInterface>& ifs, 
                std::vector<pthread_t>& threads);
int stream_data_non_blocking_queue(std::vector<SocketInterface>& ifs, 
                std::vector<pthread_t>& threads);
int stream_data_non_blocking_deque(std::vector<SocketInterface>& ifs, 
                std::vector<pthread_t>& threads);
                
/**
 * \fn void* server_thread(void *intf)
 * \brief Producer thread for a given server
 * \param intf Pointer to SocketInterface for this socket
 */
void* server_thread(void *intf);

/**
 * \fn int request_frame(SocketInterface *i, unsigned frame)
 * \brief Sends frame request to server
 * \param i SocketInterface for calling server thread
 * \param frame Frame number requested
 * \return Zero if successful, non-zero otherwise
 */
 int request_frame(SocketInterface *i, unsigned frame);

/**
 * \fn void log_frame(unsigned frame)
 * \brief Prints frame and recieve time to std err
 * \param frame Frame logged
 */
 void log_frame(unsigned frame);

double moving_average(unsigned window, std::list<double>& rates);
////////////////////////////////////////////////////////////////////////////////
#endif
