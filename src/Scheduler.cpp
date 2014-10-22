/**---------------------------------*-c++-*-------------------------------------
 * \file Scheduler.cpp
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Definitions for scheduler classes for dynamically picking servers
 *----------------------------------------------------------------------------*/
#include "Scheduler.hpp"

////////////////////////////////////////////////////////////////////////////////
// SCHEDULER
////////////////////////////////////////////////////////////////////////////////
unsigned Scheduler::pick_server()
{
  // pick random ticket
  unsigned ticket_num = rand() % tickets;

  // figure out which server has the ticket
  unsigned ticket_count = 0;
  for(unsigned index = 0; index < server_tickets.size(); index++)
  {
    ticket_count += server_tickets[index];
    if(ticket_num < ticket_count) return index;
  }

  // this shouldn't happen
  return -1;
}

////////////////////////////////////////////////////////////////////////////////
void Scheduler::reset()
{
  // set all tickets counts equal
  for(unsigned& count : server_tickets)
  {
    count = tickets / server_tickets.size();
  }
}

////////////////////////////////////////////////////////////////////////////////
// SIMPLESCHED
////////////////////////////////////////////////////////////////////////////////
void SimpleSched::update_timeout(unsigned server_index)
{
  // On timeout give a ticket to each other server
  if(server_tickets[server_index] >= (server_tickets.size() - 1))
  {
    for(unsigned index = 0; index < server_tickets.size(); index++)
    {
      if(index != server_index)
      {
        server_tickets[index]++;
      }
      else
      {
        server_tickets[index] -= server_tickets.size();
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void SimpleSched::update_latency(unsigned server_index, double latency)
{
  // scheduler is too simple to handle latency...
}

////////////////////////////////////////////////////////////////////////////////
