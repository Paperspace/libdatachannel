/******************************************************************************
**
** parse_cl.cpp
**
** Thu Aug  6 19:42:25 2020
** Linux 5.4.0-42-generic (#46-Ubuntu SMP Fri Jul 10 00:24:02 UTC 2020) x86_64
** cerik@Erik-VBox-Ubuntu (Erik Cota-Robles)
**
** Copyright (c) 2020 Erik Cota-Robles
**
** Definition of command line parser class
**
** Automatically created by genparse v0.9.3
**
** See http://genparse.sourceforge.net for details and updates
**
**
******************************************************************************/

#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32)
#include "getopt.h"
#else
#include <getopt.h>
#endif

#include "parse_cl.h"

/*----------------------------------------------------------------------------
**
** Cmdline::Cmdline ()
**
** Constructor method.
**
**--------------------------------------------------------------------------*/

Cmdline::Cmdline (int argc, char *argv[]) // ISO C++17 not allowed: throw (std::string )
{
  extern char *optarg;
  extern int optind;
  int c;

  static struct option long_options[] =
  {
    {"noStun", no_argument, NULL, 'n'},
    {"stunServers", required_argument, NULL, 's'},
	{"stunPort", required_argument, NULL, 't'},
    {"webSocketServers", required_argument, NULL, 'w'},
    {"webSocketPort", required_argument, NULL, 'x'},
    {"help", no_argument, NULL, 'h'},
	{"turnServers", required_argument, NULL, 'T'},
	{"id", required_argument, NULL, 'I'},
	{"peerId", required_argument, NULL, 'P'},
	{"message", required_argument, NULL, 'M'},
	{"messageSize", required_argument, NULL, 'S'},
	{"messageCount", required_argument, NULL, 'C'},
	{"connectionCount", required_argument, NULL, 'N'},
	{"binary", no_argument, NULL, 'B'},
	{"replyPrompt", no_argument, NULL, 'R'},
	{"echo", no_argument, NULL, 'E'},
	{"exitOnReply", no_argument, NULL, 'X'},
	{NULL, 0, NULL, 0}
  };

  _program_name += argv[0];

  /* default values */
  _n = false;
  _s = "stun.l.google.com";
  _t = 19302;
  _w = "localhost";
  _x = 8000;
  _h = false;
  _message_size = 0;
  _message_count = 1;
  _connection_count = 1;
  _send_binary = false;
  _reply_prompt = false;
  _echo = false;
  _exit_on_reply = false;

  optind = 0;
  while ((c = getopt_long (argc, argv, "s:t:w:x:T:I:P:M:S:C:N:enhvBREX", long_options, &optind)) != - 1)
    {
      switch (c)
        {
        case 'n':
          _n = true;
          break;

        case 's':
          _s = optarg;
          break;

        case 't':
          _t = atoi (optarg);
          if (_t < 0)
            {
              std::string err;
              err += "parameter range error: t must be >= 0";
              throw (std::range_error(err));
            }
          if (_t > 65535)
            {
              std::string err;
              err += "parameter range error: t must be <= 65535";
              throw (std::range_error(err));
            }
          break;

        case 'w':
          _w = optarg;
          break;

        case 'x':
          _x = atoi (optarg);
          if (_x < 0)
            {
              std::string err;
              err += "parameter range error: x must be >= 0";
              throw (std::range_error(err));
            }
          if (_x > 65535)
            {
              std::string err;
              err += "parameter range error: x must be <= 65535";
              throw (std::range_error(err));
            }
          break;

        case 'T':
			_id = optarg;
			break;

        case 'I':
			_id = optarg;
			break;

		case 'P':
			_peer_id = optarg;
			break;

        case 'M':
			_message = optarg;
			break;

		case 'C':
			_message_count = atoi(optarg);
			break;

        case 'S':
			_message_size = atoi(optarg);
			if (_message_size < 1) {
				std::string err;
				err += "message size parameter range error: x must be >= 1";
				throw(std::range_error(err));
			}
			break;

        case 'N':
			_connection_count = atoi(optarg);
			if (_connection_count < 0) {
				std::string err;
				err += "conenction count parameter range error: x must be >= 0";
				throw(std::range_error(err));
			}
			break;

		case 'B':
			_send_binary = true;
			break;

		case 'R':
			_reply_prompt = true;
			break;

		case 'E':
			_echo = true;
			break;

		case 'X':
			_exit_on_reply = true;
			break;

		case 'h':
          _h = true;
          this->usage (EXIT_SUCCESS);
          break;

        default:
          this->usage (EXIT_FAILURE);

        }
    } /* while */

  _optind = optind;
}

/*----------------------------------------------------------------------------
**
** Cmdline::usage () and version()
**
** Print out usage (or version) information, then exit.
**
**--------------------------------------------------------------------------*/

void Cmdline::usage (int status)
{
  if (status != EXIT_SUCCESS)
    std::cerr << "Try `" << _program_name << " --help' for more information.\n";
  else
    {
      std::cout << "\
usage: " << _program_name << " [ -enstwxhvTIPMCSNBREX ] \n\
libdatachannel client implementing WebRTC Data Channels with WebSocket signaling\n\
   [ -n ] [ --noStun ] (type=FLAG)\n\
          Do NOT use a stun server (overrides -s and -t).\n\
   [ -s ] [ --stunServers ] (type=STRING, default=stun.l.google.com)\n\
          Stun server URLs or IP addresses, separated by a comma; may include :portnum.\n\
   [ -t ] [ --stunPort ] (type=INTEGER, range=0...65535, default=19302)\n\
          Stun server port.\n\
   [ -w ] [ --webSocketServers ] (type=STRING, default=localhost)\n\
          Web socket server URL or IP address.\n\
   [ -x ] [ --webSocketPort ] (type=INTEGER, range=0...65535, default=8000)\n\
          Web socket server port.\n\
   [ -T ] [ --turnServers ] (type=STRING, default=none)\n\
          Turn server URLs or IP addresses, separated by a comma; may include :portnum.\n\
   [ -I ] [ --id string ] (type=STRING)\n\
          Local Id for signaling.\n\
   [ -P ] [ --peerId string ] (type=STRING)\n\
          Peer Id for signaling.\n\
   [ -M ] [ --message string ]  (type=STRING, default=Hello)\n\
          Message text.\n\
   [ -S ] [ --messsageSize size ] (type=INTEGER, range=1,...max_int)\n\
          Message size; typical maximum: 262134.\n\
   [ -C ] [ --messageCount count ] (type=INTEGER, range=-1,...max_int, default=1)\n\
          Message count; -1 means send or reply continuously.\n\
   [ -N ] [ --connectionCount count ] (type=INTEGER, range=0...max_int, default=1)\n\
          Connection count.\n\
   [ -B ] [ --binary ] (type=FLAG)\n\
          Send binary messages.\n\
   [ -R ] [ --replyPrompt ] (type=FLAG)\n\
          Prompt for message or reply.\n\
   [ -E ] [ --echo ] (type=FLAG)\n\
          Echo messages from peer.\n\
   [ -X ] [ --exitOnReply ] (type=FLAG)\n\
          Exit on reply or messageCount messages received.\n\
   [ -h ] [ --help ] (type=FLAG)\n\
          Display this help and exit.\n";
    }
  exit (status);
}

