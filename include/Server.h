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
#include <sstream>
#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////
struct timeval start_time; /// Reference time

////////////////////////////////////////////////////////////////////////////////
/**
 * \fn void log_frame(unsigned frame)
 * \brief Prints frame and recieve time to std err
 * \param frame Frame logged
 */
 void log_frame(unsigned frame);

////////////////////////////////////////////////////////////////////////////////
#endif
