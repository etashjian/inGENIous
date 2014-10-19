#include "Server.h"

using std::cout;

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
  char buf[128];
  int sequence_num = 0;

  if (argc < 2)
  {
     cout << "Usage: Server port\n";
     exit(-1);
  }

  // setup socket
  ServerSocket s(atoi(argv[1]));
  s.init();

  while (1)
  {
    // wait for next message
    s.receive(buf, 128);
    cout << "Received a datagram: " << buf << "\n";

    // check sequence number
    if(atoi(buf) == sequence_num)
    {
      sequence_num++;
    }

    snprintf(buf, 128, "%d", sequence_num);
    s.send(buf, 128);
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
