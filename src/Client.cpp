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
  vector<ClientSocket> sockets;
  int rc = 0;

  // check args
  if (argc != 2)
  {
    cerr << "Usage: Client <filename>\n";
    exit(-1);
  }

  // setup sockets
  if (parse_config_file("client.cfg", sockets))
  {
    cerr << "Config file parse failed!\n";
    exit(-1);
  }

  // initialize servers
  if ((rc = init_servers(argv[1], sockets)))
  {
    cerr << "Failed to init server " << rc << "!\n";
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
int init_servers(const char *file, std::vector<ClientSocket>& sockets)
{
  // setup initialization command pkt
  char send_buf[128], rec_buf[128];
  sprintf(send_buf, "%c%s", static_cast<char>(PKT_CMD::INIT_SERVER), file);

  // send command across all servers
  // TODO: This has room for improvment since it's done serially
  for(unsigned server_index = 0; server_index < sockets.size(); server_index++)
  {
    // send init pkt until response is received
    ClientSocket& s = sockets[server_index];
    do
    {
      if(s.send(send_buf, 128)) return server_index;
      bzero(rec_buf, 128);
    }
    while (s.receive(rec_buf, 128));

    // check response
    if(static_cast<PKT_CMD>(*rec_buf) != PKT_CMD::SERVER_READY)
    {
      return server_index;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
