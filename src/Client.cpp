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
  if (parse_config_file("client.cfg", sockets))
  {
    cerr << "Config file parse failed!\n";
    exit(-1);
  }

  // launch threads
  vector<SocketInterface> ifs;
  vector<pthread_t> threads(sockets.size());
  if (start_servers(ifs, threads, sockets, argv[1]))
  {
    cerr << "Failed to start servers!\n";
    exit(-1);
  }

  // stream file
  if (stream_file(ifs, threads))
  {
    cerr << "Failed to stream file!\n";
    exit(-1);
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
    sockets.back().init();
    sockets.back().configure_timeout(2, 0);
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
    ifs.push_back(SocketInterface(file, sockets[i]));
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
    while(!ifs[i].ready);
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

  return 0; // exit successfully
}

////////////////////////////////////////////////////////////////////////////////
void* server_thread(void *intf)
{
  SocketInterface *i = static_cast<SocketInterface*>(intf);

  // try to init server
  if(init_server(i)) pthread_exit(nullptr);

  pthread_exit(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
int init_server(SocketInterface *i)
{
  // setup initialization command pkt
  char send_buf[128], rec_buf[128];
  sprintf(send_buf, "%c%s", static_cast<char>(PKT_CMD::INIT_SERVER), i->file);

  // send init pkt until response is received
  do
  {
    if(i->socket.send(send_buf, 128)) return -1;
    bzero(rec_buf, 128);
  }
  while (i->socket.receive(rec_buf, 128));

  // check response
  if(static_cast<PKT_CMD>(*rec_buf) != PKT_CMD::SERVER_READY)
  {
    return -1;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
