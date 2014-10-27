/**---------------------------------*-c++-*-------------------------------------
 * \file Scheduler.h
 * \author Edward Tashjian, Addison Floyd
 * \date October 2015
 * \brief Declarations for scheduler classes for dynamically picking servers
 *----------------------------------------------------------------------------*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
/**
 * \class Scheduler
 * \brief Base class for lottery schedulers for dynamically choosing servers
 */
class Scheduler
{
 public:
  /**
   * \fn Scheduler(unsigned num_servers, unsigned num_tickets)
   * \brief Basic constructor for lottery based scheduler
   * \param num_server Number of servers to schedule
   * \param tickets_per_server Number of tickets per server
   */
  Scheduler(unsigned num_servers, unsigned tickets_per_server)
    : tickets(num_servers * tickets_per_server)
    , server_tickets(num_servers, tickets_per_server)
    {}

  /**
   * \fn unsigned pick_server()
   * \brief Draws lottery number and picks corresponding server
   * \return index of picked server
   */
  virtual unsigned pick_server();

  /**
   * \fn void update_timeout()
   * \brief Notifies scheduler of a timeout by a given server
   * \param server_index Index of server that timedout
   */
  virtual void update_timeout(unsigned server_index)=0;

  /**
   * \fn void update_latency(unsigned server_index, double latency)
   * \brief Notifies scheuler of latency of transmission between server
   * \param server_index Index of server updated
   * \param latency Latency (ms) of transmission
   */
  virtual void update_latency(unsigned server_index, double latency)=0;

  /**
   * \fn void reset()
   * \brief Resets all servers to equal standing
   */
  void reset();

 protected:
  unsigned tickets; /// Total tickets in system
  std::vector<unsigned> server_tickets; /// Tickets held by each server

};

////////////////////////////////////////////////////////////////////////////////
/**
 * \class SimpleSched
 * \brief Basic scheduler for testing. Probably not worth much
 */
class SimpleSched : Scheduler
{
 public:
  /**
   * \fn Scheduler(unsigned num_servers, unsigned num_tickets)
   * \brief Basic constructor SimpleShed
   * \param num_server Number of servers to schedule
   * \param tickets_per_server Number of tickets per server
   */
  SimpleSched(unsigned num_servers, unsigned tickets_per_server)
    : Scheduler(num_servers, tickets_per_server)
    {}

  /**
   * \fn void update_timeout()
   * \brief Notifies scheduler of a timeout by a given server
   * \param server_index Index of server that timedout
   */
  virtual void update_timeout(unsigned server_index);

  /**
   * \fn void update_latency(unsigned server_index, double latency)
   * \brief Notifies scheuler of latency of transmission between server
   * \param server_index Index of server updated
   * \param latency Latency (ms) of transmission
   */
  virtual void update_latency(unsigned server_index, double latency);

};

////////////////////////////////////////////////////////////////////////////////
#endif
