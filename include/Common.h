/**---------------------------------*-c++-*-------------------------------------
 * \file Common.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015 
 * \brief Common enums and constants
 *----------------------------------------------------------------------------*/

#ifndef COMMON_H
#define COMMON_H

////////////////////////////////////////////////////////////////////////////////
enum class ClientCmd : char
{
  REQUEST,
  ACK
};

////////////////////////////////////////////////////////////////////////////////
const unsigned SERVER_HDR_SIZE = sizeof(unsigned); 
const unsigned SERVER_DATA_SIZE = 1024; // size of frame
const unsigned SERVER_PKT_SIZE = SERVER_HDR_SIZE + SERVER_DATA_SIZE;
const unsigned CLIENT_HDR_SIZE = sizeof(ClientCmd);
const unsigned CLIENT_DATA_SIZE = sizeof(unsigned); 
const unsigned CLIENT_PKT_SIZE = CLIENT_HDR_SIZE + CLIENT_DATA_SIZE;

const unsigned DEFAULT_MAX_QUEUE_SIZE = 1;
const unsigned DEFAULT_INIT_WINDOW_SIZE = 1;
////////////////////////////////////////////////////////////////////////////////
#endif
