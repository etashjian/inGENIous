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

  // start serving
  while (1)
  {
    // wait for start message
    if(s.receive(buf, 128)) exit(-1);
    cout << "Received a datagram: ";

    // choose appropriate action based on packet header
    PKT_CMD cmd = static_cast<PKT_CMD>(*buf);
    switch(cmd)
    {
      // Server initialization
      case PKT_CMD::INIT_SERVER:
        if(init_server(s, in, buf)) exit(-1);
        break;

      // Frame data request
      case PKT_CMD::FRAME_REQ:
        if(send_frame_data(s, in, buf)) exit(-1);
        break;

      // Bad command
      default:
        // drop any other pkts
        cout << "invalid command.\n";
        break;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int init_server(ServerSocket& s, ifstream& in, char *buf)
{
  // close any previously opened file
  if(in.is_open()) in.close();

  // grab movie name and try to open
  char *filename = buf + PKT_HDR_SIZE;
  in.open(filename);

  // send appropriate response
  if(in.is_open())
  {
    cout << "opened file " << filename << endl;

    // calc number of frames
    in.seekg(0, in.end);
    unsigned length = in.tellg();
    in.seekg(0, in.beg);

    // format message
    sprintf(buf, "%c", PKT_CMD::SERVER_READY);
    memcpy(buf + PKT_HDR_SIZE, &length, sizeof(unsigned));
  }
  else
  {
    cout << "could not open " << filename << endl;
    sprintf(buf, "%c", PKT_CMD::INVALID_INIT);
  }

  return s.send(buf, PKT_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
int send_frame_data(ServerSocket& s, ifstream& in, char *buf)
{

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
