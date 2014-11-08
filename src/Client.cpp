/**---------------------------------*-c++-*-------------------------------------
 * \file Client.cpp
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Implenetation of Client executable
 *----------------------------------------------------------------------------*/

#include "Client.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
  // check args
  if (argc != 2)
  {
    cerr << "Usage: Client <#frames>\n";
    exit(-1);
  }

  // setup sockets
  vector<ClientSocket> sockets;
  cout << "Setting up sockets... " << flush;
  if (parse_config_file("Configuration/server.cfg", sockets))
  {
    cerr << "Config file parse failed!\n";
    exit(-1);
  }
  cout << "done\n";

  // launch threads
  cout << "Launching threads... " << flush;
  vector<SocketInterface> ifs(sockets.size());
  vector<pthread_t> threads(sockets.size());
  if(start_servers(ifs, threads, sockets, argv[1]))
  {
    cerr << "Failed to start servers!\n";
    exit(-1);
  }
  cout << "done\n";

  // stream file
  cout << "Streaming file... " << flush;
  if (stream_data(ifs, threads, atoi(argv[1])))
  {
    cerr << "Failed to stream file!\n";
    exit(-1);
  }
  cout << "done\n";

  // join all threads
  cout << "Joining threads... " << flush;
  for(unsigned i = 0; i < threads.size(); i++)
  {
    pthread_cond_signal(&ifs[i].empty);
    pthread_join(threads[i], nullptr);
  }
  cout << "done\n";

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
int start_servers(vector<SocketInterface>& ifs,
                  vector<pthread_t>& threads,
                  vector<ClientSocket>& sockets,
                  char *file)
{
  // Initialize threads
  int rc = 0;
  for(unsigned i = 0; i < sockets.size(); i++)
  {
    ifs[i].socket = &sockets[i];
    ifs[i].file = file;
    rc = pthread_create(&threads[i], nullptr, server_thread, (void*) &ifs[i]);
    if(rc)
    {
      cerr << "Error code " << rc << " occured creating thread " << i << "!\n";
      return -1;
    }
  }

  return 0; // exit successfully
}

////////////////////////////////////////////////////////////////////////////////
int stream_data(vector<SocketInterface>& ifs,
                vector<pthread_t>& threads,
                unsigned num_frames)
{
  // initialize reference time
  if(gettimeofday(&start_time, nullptr))
  {
    cerr << "FAILED TO INITIALIZE REFERENCE TIME\n";
    return -1;
  }

  // for now just evenly spread requests across servers
  unsigned server = 0;
  for(unsigned index = 0; index < num_frames; index++)
  {
    // push request
    pthread_mutex_lock(&ifs[server].lock);
    while(ifs[server].frame_reqs.size() == MAX_QUEUE_SIZE)
      pthread_cond_wait(&ifs[server].full, &ifs[server].lock);

    ifs[server].frame_reqs.push(index);
    pthread_mutex_unlock(&ifs[server].lock);
    pthread_cond_signal(&ifs[server].empty);

    // update pos and server
    server = (server + 1) % ifs.size();
  }

  // signal done to all threads
  for(unsigned i = 0; i < ifs.size(); i++)
  {
    while(!ifs[i].frame_reqs.empty());
    ifs[i].ready = 0;
  }

  return 0; // exit successfully
}

////////////////////////////////////////////////////////////////////////////////
void* server_thread(void *intf)
{
  SocketInterface *i = static_cast<SocketInterface*>(intf);
  unsigned frame = 0;
  char rec_buf[PKT_SIZE];
  set<unsigned> outstanding_frames, oo_frames;
  unsigned window_size = INIT_WINDOW_SIZE;

  // set thread to ready
  i->ready = 1;

  // keep waiting for packets until client signals close
  while(i->ready)
  {
    // if window not full and packets available, get/send them
    if(outstanding_frames.size() < window_size && !i->frame_reqs.empty())
    {
      // get frame from queue
      pthread_mutex_lock(&i->lock);
      frame = i->frame_reqs.front();
      i->frame_reqs.pop();
      pthread_mutex_unlock(&i->lock);
      pthread_cond_signal(&i->full);

      // build/send request packet
      if(request_frame(i, frame)) pthread_exit(nullptr);

      // add frame to outstanding frames
      outstanding_frames.insert(frame);
    }
    // if no frames outstanding and no requests, wait
    else if(outstanding_frames.empty() && i->frame_reqs.empty())
    {
      pthread_mutex_lock(&i->lock);
      while(i->frame_reqs.empty() && i->ready)
        pthread_cond_wait(&i->empty, &i->lock);

      if(!i->ready) break;
    }
    // otherwise wait for packets
    else
    {
      // get packet
      bzero(rec_buf, PKT_SIZE);
      if(i->socket->receive(rec_buf, PKT_SIZE))
      {
        // for now just resend first in line pkt
        if(request_frame(i, *outstanding_frames.begin())) pthread_exit(nullptr);
        continue;
      }

      // remove frame from outstanding packets
      memcpy(&frame, rec_buf, sizeof(unsigned));

      // make sure frame number received is valid
      if(outstanding_frames.find(frame) != outstanding_frames.end())
      {
        // if frame received is next in order, retire it and any out of order
        // frames
        if(frame == *outstanding_frames.begin())
        {
          outstanding_frames.erase(frame);
          outstanding_frames.erase(oo_frames.begin(), oo_frames.end());
        }
        // otherwise, add to set of out of order frames received
        else
        {
          oo_frames.insert(frame);
        }

        // record time stamp (printed to std err)
        log_frame(frame);
      }
    }
  }

  pthread_exit(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
int request_frame(SocketInterface *i, unsigned frame)
{
  char send_buf[PKT_SIZE];
  memcpy(send_buf, &frame, sizeof(unsigned));
  int rc = i->socket->send(send_buf, PKT_SIZE);
  if(rc) cout << "Failed to send request to server!\n";
  return rc;
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
