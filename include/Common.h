/**---------------------------------*-c++-*-------------------------------------
 * \file Common.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015 
 * \brief Common enums and constants
 *----------------------------------------------------------------------------*/

#ifndef COMMON_H
#define COMMON_H

////////////////////////////////////////////////////////////////////////////////
const unsigned FRAME_SIZE = 128; /// Size of move frame
const unsigned PKTS_PER_FRAME = 1; /// Number of pkts frame is split across
const unsigned PKT_HDR_SIZE = sizeof(unsigned); // Index number header
const unsigned PKT_DATA_SIZE = FRAME_SIZE / PKTS_PER_FRAME;
const unsigned PKT_SIZE = PKT_HDR_SIZE + PKT_DATA_SIZE; /// Total size of pkt

const unsigned DEFAULT_MAX_QUEUE_SIZE = 5;
const unsigned DEFAULT_INIT_WINDOW_SIZE = 5;
////////////////////////////////////////////////////////////////////////////////
#endif
