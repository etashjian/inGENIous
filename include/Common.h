/**---------------------------------*-c++-*-------------------------------------
 * \file Common.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015 
 * \brief Common enums and constants
 *----------------------------------------------------------------------------*/

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
