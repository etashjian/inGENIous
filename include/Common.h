/**---------------------------------*-c++-*-------------------------------------
 * \file Common.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015 
 * \brief Common enums and constants
 *----------------------------------------------------------------------------*/

#ifndef COMMON_H
#define COMMON_H

////////////////////////////////////////////////////////////////////////////////
/**
 * \enum PKT_CMD
 * \brief All packets start with a PKT_CMD to describe the purpose of pkt
 */
enum class PKT_CMD : char 
{
  INIT_SERVER, // Client signal to server to get ready
  SERVER_READY, // Server acknowledgment to INIT_SERVER
  INVALID_INIT, // Server response to INIT_SERVER, indicating init failed
  FRAME_REQ, // Client request for frame data
  FRAME_DATA // Server reponse with data
};

////////////////////////////////////////////////////////////////////////////////
const unsigned FRAME_SIZE = 1024; /// Size of move frame
const unsigned PKTS_PER_FRAME = 1; /// Number of pkts frame is split across
const unsigned PKT_HDR_SIZE = sizeof(PKT_CMD) + sizeof(unsigned);
const unsigned PKT_DATA_SIZE = FRAME_SIZE / PKTS_PER_FRAME;
const unsigned PKT_SIZE = PKT_HDR_SIZE + PKT_DATA_SIZE; /// Total size of pkt
const unsigned INDEX_POS = sizeof(PKT_CMD);

const unsigned MAX_QUEUE_SIZE = 5; // Max size of request thread queue
////////////////////////////////////////////////////////////////////////////////
#endif
