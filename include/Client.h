/**---------------------------------*-c++-*-------------------------------------
 * \file Client.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Header for Client executable for moving streaming project
 *----------------------------------------------------------------------------*/
#include "Socket.h"
#include "Common.h"
#include <iostream>
#include <fstream>
#include <vector>

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
 * \fn int init_servers(const char *file, std::vector<ClientSocket>& sockets)
 * \brief Sends init messages to all server to open and prepare to send file
 * \param file Name of file for servers to open and send
 * \param sockets Vector of sockets to send from
 * \return zero if successful, otherwise index of server if a server fails
 */
int init_servers(const char *file, std::vector<ClientSocket>& sockets);
