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
#include <queue>
#include <fstream>
#include <sstream>
#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////
struct timeval start_time; /// Reference time

////////////////////////////////////////////////////////////////////////////////
/**
 * \fn void init(ServerSocket& s)
 * \brief Initializes server
 * \param s Reference to ServerSocket used by server
 * \return Zero if successful, non-zero otherwise
 */
int init(ServerSocket& s);

/**
 * \fn int send_frame(ServerSocket& s, unsigned frame)
 * \brief Send data for frame to client
 * \param s ServerSocket to send with
 * \param frame Frame number
 * \return Zero if successful, non-zero otherwise
 */
int send_frame(ServerSocket& s, unsigned frame);

/**
 * \fn void log_frame(unsigned frame)
 * \brief Prints frame and recieve time to std err
 * \param frame Frame logged
 */
 void log_frame(unsigned frame);

////////////////////////////////////////////////////////////////////////////////
#endif
