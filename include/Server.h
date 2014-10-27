/**---------------------------------*-c++-*-------------------------------------
 * \file Server.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Header for Server executable in movie streaming project
 *----------------------------------------------------------------------------*/

#ifndef SERVER_H
#define SERVER_H

#include "Common.h"
#include "Socket.h"
#include <iostream>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
/**
 * \fn int init_server(ServerSocket& s, ifstream& in, char *buf)
 * \brief Respondes to server init command
 * \param s Server's socket
 * \param in File input stream
 * \param buf Message buffer
 * \return zero on success, non-zero on fail
 */
int init_server(ServerSocket& s, std::ifstream& in, char *buf);

/**
 * \fn int send_frame(ServerSocket& s, ifstream& in, char *buf)
 * \brief Sends requested frame data to client
 * \param s Server's socket
 * \param in File input stream
 * \param buf Message buffer
 * \return zero on success, non-zero on fail
 */
int send_frame_data(ServerSocket& s, std::ifstream& in, char *buf);
////////////////////////////////////////////////////////////////////////////////
#endif
