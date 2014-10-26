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

  // wait for ready signals
  for(unsigned i = 0; i < sockets.size(); i++)
  {
    while(!ifs[i].ready && !ifs[i].error);
    if(ifs[i].error)
    {
      cerr << "Server " << i << " failed to initialize!\n";
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
    ifs[server].frame_reqs.push(index);
    pthread_mutex_unlock(&ifs[server].lock);

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

  // try to init server
  if(init_server(i)) pthread_exit(nullptr);

  // keep waiting for packets until client signals close
  while(i->ready)
  {
    // wait for next packet or exit condition
    while(i->ready && i->frame_reqs.empty());
    if(!i->ready) break;

    // get next frame
    pthread_mutex_lock(&i->lock);
    frame = i->frame_reqs.front();
    i->frame_reqs.pop();
    pthread_mutex_unlock(&i->lock);

    // build/send request packet
    *send_buf = static_cast<char>(PKT_CMD::FRAME_REQ);
    memcpy(send_buf + INDEX_POS, &frame, sizeof(unsigned));
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
int init_server(SocketInterface *i)
{
  // setup initialization command pkt
  char send_buf[PKT_SIZE], rec_buf[PKT_SIZE];
  *send_buf = static_cast<char>(PKT_CMD::INIT_SERVER);
  sprintf(send_buf + PKT_HDR_SIZE, "%s", i->file);

  // send init pkt until response is received
  do
  {
    if(i->socket->send(send_buf, PKT_SIZE)) return -1;
    bzero(rec_buf, PKT_SIZE);
  }
  while (i->socket->receive(rec_buf, PKT_SIZE));

  // check response
  if(static_cast<PKT_CMD>(*rec_buf) != PKT_CMD::SERVER_READY)
  {
    i->error = 1;
    return -1;
  }

  // pull out file size
  memcpy(&i->file_size, rec_buf + PKT_HDR_SIZE, sizeof(unsigned));

  i->ready = 1;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
