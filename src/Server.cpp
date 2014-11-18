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
  char rec_buf[CLIENT_PKT_SIZE]; // send/receive bufs
  ifstream in; // file input stream
  unsigned num_frames, next_frame, acked_frame; // frame info
  queue<unsigned> outstanding_frames;
  unsigned window_size = 10;

  // check args
  if (argc < 2)
  {
     cout << "Usage: Server <port>\n";
     exit(-1);
  }

  // init server
  ServerSocket s(atoi(argv[1]));
  if(init(s)) exit(-1);

  // start serving
  while (1)
  {
    // wait for request from client
    bzero(rec_buf, CLIENT_PKT_SIZE);
    if(s.receive(rec_buf, CLIENT_PKT_SIZE)) exit(-1);

    // command needs to be a request, drop it if not
    if(static_cast<ClientCmd>(*rec_buf) != ClientCmd::REQUEST) continue;

    // get info from client request
    memcpy(&num_frames, rec_buf + CLIENT_HDR_SIZE, CLIENT_DATA_SIZE);
    next_frame = 0;

    while(next_frame < num_frames)
    {
      // if room in window send a message
      if(outstanding_frames.size() < window_size)
      {
        send_frame(s, next_frame);
        outstanding_frames.push(next_frame++);
      }
      // otherwise wait for acks
      else if(s.receive(rec_buf, CLIENT_PKT_SIZE) == 0)
      {
        if(static_cast<ClientCmd>(*rec_buf) != ClientCmd::ACK) continue;
        memcpy(&acked_frame, rec_buf + CLIENT_HDR_SIZE, CLIENT_DATA_SIZE);
        while(outstanding_frames.front() < acked_frame)
          outstanding_frames.pop();
      }
      // timeout
      else
      {
        cout << "TIMEOUT!\n";
        send_frame(s, outstanding_frames.front());
      }
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int init(ServerSocket& s)
{
  cout << "Initializing server... " << flush;

  // init socket
  if(s.init())
  {
    cout << "Failed to initialize socket!\n";
    return -1;
  }

  // random seed
  srand(time(nullptr));

  // initialize reference time
  if(gettimeofday(&start_time, nullptr))
  {
    cout << "Failed to initialize reference time!\n";
    return -1;
  }

  cout << "ready!\n";
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int send_frame(ServerSocket& s, unsigned frame)
{
  char send_buf[SERVER_PKT_SIZE];

  // build message (using random letters for easy visual checking)
  memcpy(send_buf, &frame, SERVER_HDR_SIZE);
  for(unsigned i = 0; i < SERVER_DATA_SIZE; i++)
    *(send_buf + SERVER_HDR_SIZE + i) = static_cast<char>('a' + rand() % 26);

  // send message
  if(s.send(send_buf, SERVER_PKT_SIZE)) return -1;

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
