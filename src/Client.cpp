#include "Client.h"

using std::cout;

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
  char buf[128];
  int sequence_num = 0;

  // check args
  if (argc != 3)
  {
    cout << "Usage: Client hostaname port\n";
    exit(-1);
  }

  // setup socket
  ClientSocket s(atoi(argv[1]), argv[2]);
  s.init();

  // send/receive sequence numbers
  while(1)
  {
    bool timed_out = false;
    do
    {
      // check timeout conditions
      if(timed_out) cout << "TIMEDOUT!\n";
      timed_out = true;

      // send next sequence number
      cout << "Sending sequence number: " << sequence_num << "\n";
      bzero(buf,128);
      snprintf(buf, 128, "%d", sequence_num);
      s.send(buf, 128);

    } while (s.receive(buf, 128));

    // check sequence number
    cout << "Receiver waiting for: " << buf << "\n";
    if(sequence_num < atoi(buf)) sequence_num++;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
