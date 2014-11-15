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
#include <set>
#include <unordered_set>
#include <pthread.h>
#include <sys/time.h>
#include <sstream>
#include <getopt.h>

////////////////////////////////////////////////////////////////////////////////
struct timeval start_time; /// Reference time
unsigned init_window_size = DEFAULT_INIT_WINDOW_SIZE;
unsigned max_queue_size = DEFAULT_MAX_QUEUE_SIZE;
unsigned num_frames = 0;

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
 * \fn void log_frame(unsigned frame)
 * \brief Prints frame and recieve time to std err
 * \param frame Frame logged
 */
 void log_frame(unsigned frame);

////////////////////////////////////////////////////////////////////////////////
#endif
