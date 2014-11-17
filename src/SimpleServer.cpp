/**---------------------------------*-c++-*-------------------------------------
 * \file Server.cpp
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Definitions for server exectuable
 *----------------------------------------------------------------------------*/

#include "SimpleServer.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
  char buf[RESP_PKT_SIZE]; // send/receive buffer
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

  // initialize reference time
  if(gettimeofday(&start_time, nullptr))
  {
    cerr << "FAILED TO INITIALIZE REFERENCE TIME\n";
    return -1;
  }

  // start serving
  unsigned frame = 0;
  while (1)
  {
    bzero(buf, RESP_PKT_SIZE);
    memcpy(buf, &frame, sizeof(frame));

    // build random message (using letters for easy visual checking)
    for(unsigned i = 0; i < PKT_DATA_SIZE; i++)
      *(buf + PKT_HDR_SIZE + i) = static_cast<char>('a' + rand()%26);

    // send response
    if(s.send(buf, RESP_PKT_SIZE))
    {
      cout << "FAILED TO SEND PACKET!\n";
      return -1;
    }
    log_frame(frame);
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
void log_frame(unsigned frame)
{
  struct timeval time;
  if(gettimeofday(&time, nullptr))
  {
    cerr << "FAILED TO READ WALL TIME!\n";
    pthread_exit(nullptr);
  }
  stringstream ss;
  ss << (double)time.tv_sec + (double)time.tv_usec * .000001 -
        (double)start_time.tv_sec - (double)start_time.tv_usec * .000001
     << " " << frame << endl;
  cerr << ss.str();
}

////////////////////////////////////////////////////////////////////////////////
