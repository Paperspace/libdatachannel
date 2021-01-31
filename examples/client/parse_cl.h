/******************************************************************************
**
** parse_cl.h
**
** Thu Aug  6 19:42:25 2020
** Linux 5.4.0-42-generic (#46-Ubuntu SMP Fri Jul 10 00:24:02 UTC 2020) x86_64
** cerik@Erik-VBox-Ubuntu (Erik Cota-Robles)
**
** Copyright (c) 2020 Erik Cota-Robles
**
** Header file for command line parser class
**
** Automatically created by genparse v0.9.3
**
** See http://genparse.sourceforge.net for details and updates
**
******************************************************************************/

#ifndef CMDLINE_H
#define CMDLINE_H

#include <iostream>
#include <string>

/*----------------------------------------------------------------------------
**
** class Cmdline
**
** command line parser class
**
**--------------------------------------------------------------------------*/

class Cmdline
{
private:
  /* parameters */
  bool _n;
  std::string _s;
  int _t;
  std::string _turn_servers;
  std::string _w;
  int _x;
  std::string _id;
  std::string _peer_id;
  std::string _message;
  int _message_size;
  int _message_count;
  int _connection_count;
  bool _send_binary;
  bool _reply_prompt;
  bool _echo;
  bool _exit_on_reply;
  bool _h;

  /* other stuff to keep track of */
  std::string _program_name;
  int _optind;

public:
  /* constructor and destructor */
  Cmdline (int, char **); // ISO C++17 not allowed: throw (std::string);
  ~Cmdline (){}

  /* usage function */
  void usage (int status);

  /* return next (non-option) parameter */
  int next_param () { return _optind; }

  bool noStun () const { return _n; }
  std::string stunServers () const { return _s; }
  std::string turnServers () const { return _turn_servers; }
  int stunPort () const { return _t; }
  std::string webSocketServer () const { return _w; }
  int webSocketPort () const { return _x; }
  std::string id () const { return _id; }
  std::string peerId () const { return _peer_id; }
  std::string message () const { return _message; }
  int messageSize () const { return _message_size; }
  int messageCount () const { return _message_count; }
  int sendContinuously () const { return _message_count < 0; }
  int connectionCount () const { return _connection_count; }
  bool sendBinary () const { return _send_binary; }
  bool replyPrompt () const { return _reply_prompt; }
  bool echo () const { return _echo; }
  bool exitOnReply () const { return _exit_on_reply; }
  bool h () const { return _h; }
};

#endif
