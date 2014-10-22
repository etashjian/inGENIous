/**---------------------------------*-c++-*-------------------------------------
 * \file Server.cpp
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Definitions for server exectuable
 *----------------------------------------------------------------------------*/
#include "Server.h"

using std::cout;
using std::ifstream;

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
  char buf[128];
  ifstream in;

  if (argc < 2)
  {
     cout << "Usage: Server <port>\n";
     exit(-1);
  }

  // setup socket
  ServerSocket s(atoi(argv[1]));
  s.init();

  // start serving
  while (1)
  {
    // wait for start message
    s.receive(buf, 128);
    cout << "Received a datagram: " << buf << "\n";

    PKT_CMD cmd = static_cast<PKT_CMD>(*buf);
    switch(cmd)
    {
      case PKT_CMD::INIT_SERVER:
      {
        // grab movie name and try to open
        char *filename = buf + 1;
        in.open(filename);

        // send appropriate response
        if(in.is_open()) sprintf(buf, "%c", PKT_CMD::SERVER_READY);
        else sprintf(buf, "%c", PKT_CMD::INVALID_INIT);

        s.send(buf, 128);
      }
      break;

      case PKT_CMD::FRAME_REQ:
        // do something interesting
        break;

      default:
        // drop any other pkts
        break;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
