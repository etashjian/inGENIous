/**---------------------------------*-c++-*-------------------------------------
 * \file Server.cpp
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Definitions for server exectuable
 *----------------------------------------------------------------------------*/

#include "Server.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
  char buf[PKT_SIZE]; // send/receive buffer
  ifstream in; // file input stream

  // check args
  if (argc < 2)
  {
     cout << "Usage: Server <port>\n";
     exit(-1);
  }

  // setup socket
  cout << "Opening socket... " << flush;
  ServerSocket s(atoi(argv[1]));
  if(s.init()) exit(-1);
  cout << "ready!\n";


  // random seed
  srand(time(nullptr));


  // start serving
  while (1)
  {
    // wait for start message
    if(s.receive(buf, 128)) exit(-1);
    cout << "Received a datagram: " << flush;

    // pull out index
    unsigned index;
    memcpy(&index, buf, PKT_HDR_SIZE);

    // build random message (using letters for easy visual checking)
    for(unsigned i = 0; i < PKT_DATA_SIZE; i++)
      *(buf + PKT_HDR_SIZE + i) = static_cast<char>('a' + rand()%26);

    // send response
    cout << "sending index " << index << endl;
    if(s.send(buf, PKT_SIZE))
    {
      cout << "FAILED TO SEND PACKET!\n";
      return -1;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
