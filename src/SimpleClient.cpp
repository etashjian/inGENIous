/**---------------------------------*-c++-*-------------------------------------
 * \file Client.cpp
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Implenetation of Client executable
 *----------------------------------------------------------------------------*/

#include "SimpleClient.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
  // check args
  if (parse_cmdline(argc, argv)) exit(-1);

  // setup sockets
  vector<ClientSocket> sockets;
  cout << "Setting up sockets... " << flush;
  if (parse_config_file("Configuration/server.cfg", sockets))
  {
    cerr << "Config file parse failed!\n";
    exit(-1);
  }
  cout << "done\n";

  unsigned window_size = init_window_size;
  queue<unsigned> outstanding_frames;
  unordered_set<unsigned> oo_frames;
  unsigned frame = 1;
  unsigned rec_frame = 0;
  ClientSocket s = sockets[0];

  // initialize reference time
  if(gettimeofday(&start_time, nullptr))
  {
    cerr << "FAILED TO INITIALIZE REFERENCE TIME\n";
    return -1;
  }

  while(1)  
  {
    if(outstanding_frames.size() < window_size && frame <= num_frames)
    {
      char send_buf[PKT_SIZE];
      memcpy(send_buf, &frame, sizeof(unsigned));
      int rc = s.send(send_buf, PKT_SIZE);
      if(rc) exit(-1);
      log_frame(frame);
      outstanding_frames.push(frame);
      frame++;
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    else
    {
      // get packet
      char rec_buf[PKT_SIZE];
      bzero(rec_buf, PKT_SIZE);
      if(s.receive(rec_buf, PKT_SIZE))
      {
        // for now just resend first in line pkt
        exit(-1);
      }

      // remove frame from outstanding packets
      memcpy(&rec_frame, rec_buf, sizeof(unsigned));

      // if frame received is next in order
      if(rec_frame == outstanding_frames.front())
      {
        log_frame(rec_frame);
        if(rec_frame == num_frames) exit(0);
        outstanding_frames.pop();
        while(outstanding_frames.size() && 
              oo_frames.find(outstanding_frames.front()) != oo_frames.end())
        {
          oo_frames.erase(outstanding_frames.front());
          outstanding_frames.pop();
        }
      }
      // otherwise, add to set of out of order frames received
      else
      {
        log_frame(rec_frame);
        oo_frames.insert(rec_frame);
      }
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
void print_usage()
{
  cout << "usage: Client -n <# frames> [-q <max queue size>] "
       << "[-w <window size>]\n";
}

////////////////////////////////////////////////////////////////////////////////
int parse_cmdline(int argc, char **argv)
{
  int c;

  while(1)
  {
    // command line options
    static struct option program_options[] =
    {
      {"Number of Frames to xmit", required_argument, 0, 'n'},
      {"Window Size", required_argument, 0, 'w'},
      {"Request Thread Queue Size", required_argument, 0, 'q'},
      {0, 0, 0, 0}
    };

    int option_index = 0;
    c = getopt_long(argc, argv, "n:w:q:", program_options, &option_index);

    if(c == -1) break; // detect quit condition
  
    switch(c)
    {
      case 'n':
        num_frames = atoi(optarg);
        break;

      case 'w':
        init_window_size = atoi(optarg);
        break;

      case 'q':
        max_queue_size = atoi(optarg);
        break;

      case '?':  
        print_usage();
        return -1;

      default:
        print_usage();
        return -1;
    }
  }

  // check that some number of frames has been specified
  if(num_frames <= 0)
  {
    cout << "Must send 1 or more frames!\n";
    print_usage();
    return -1;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int parse_config_file(const char *filename, vector<ClientSocket>& sockets)
{
  // try to open config file
  ifstream cfg(filename);
  if (!cfg.is_open()) return -1;

  // read out server hostnames and ports (one per line)
  string host_info;
  while (getline(cfg, host_info))
  {
    // pull out hostname and port
    string hostname = host_info.substr(0, host_info.find(" "));
    string port = host_info.substr(host_info.find(" ") + 1);

    // create corresponding socket
    sockets.push_back(ClientSocket(atoi(port.c_str()), hostname.c_str()));
    if(sockets.back().init() || sockets.back().configure_timeout(2, 0))
      return -1;
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
