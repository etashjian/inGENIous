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
    cerr << "Usage: Client <filename>\n";
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
  if (stream_file(ifs, threads))
  {
    cerr << "Failed to stream file!\n";
    exit(-1);
  }
  cout << "done\n";

  // join all threads
  for(unsigned i = 0; i < threads.size(); i++)
    pthread_join(threads[i], nullptr);

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
int stream_file(vector<SocketInterface>& ifs, vector<pthread_t>& threads)
{
  // for now just evenly spread requests across servers
  unsigned index = 0;
  unsigned server = 0;
  while(index * PKT_DATA_SIZE < ifs[0].file_size)
  {
    // push request
    pthread_mutex_lock(&ifs[server].lock);
    while(ifs[server].frame_reqs.size() == MAX_QUEUE_SIZE)
      pthread_cond_wait(&ifs[server].full, &ifs[server].lock);

    ifs[server].frame_reqs.push(index);
    pthread_mutex_unlock(&ifs[server].lock);
    pthread_cond_signal(&ifs[server].empty);

    // update pos and server
    index++;
    server = (server + 1) % ifs.size();
  }

  // signal done to all threads
  for(unsigned i = 0; i < ifs.size(); i++)
    ifs[i].ready = 0;

  return 0; // exit successfully
}

////////////////////////////////////////////////////////////////////////////////
void* server_thread(void *intf)
{
  SocketInterface *i = static_cast<SocketInterface*>(intf);
  unsigned frame = 0;
  char send_buf[PKT_SIZE], rec_buf[PKT_SIZE];

  // set thread to ready
  i->ready = 1;

  // keep waiting for packets until client signals close
  while(i->ready)
  {
    // wait for next packet or exit condition
    pthread_mutex_lock(&i->lock);
    while(i->frame_reqs.empty() && i->ready)
      pthread_cond_wait(&i->empty, &i->lock);

    if(!i->ready) break; // kill thread when ready is unset

    // get next frame
    frame = i->frame_reqs.front();
    i->frame_reqs.pop();
    pthread_mutex_unlock(&i->lock);
    pthread_cond_signal(&i->full);

    // build/send request packet
    memcpy(send_buf, &frame, sizeof(unsigned));
    do
    {
      if(i->socket->send(send_buf, PKT_SIZE)) pthread_exit(nullptr);
      bzero(rec_buf, PKT_SIZE);
    }
    while(i->socket->receive(rec_buf, PKT_SIZE));

    // TODO CHECK THIS
  }

  pthread_exit(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
