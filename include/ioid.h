/** File ioid.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#ifndef __IOID_H__
#define __IOID_H__

#include "geod.h"

#if WINDOWS

#include <winsock.h>

#elif LINUX || MINIX

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif

class ioid
{
   std::vector<double> msgbuf;
#if WINDOWS
   SOCKET udpSocket;
   int tid;
#elif LINUX || MINIX
   int udpSocket;   
   pid_t tid;
#endif
   
   struct sockaddr_in srv_addr;
   
   public:
      ioid();
      ~ioid();
      void setlen(int l);
      void io_open();
      void io_close();
      int write(char *buf, int len);
      int read(char *buf, int len);
};


struct client_id
{
  sockaddr_in c_addr;
#if LINUX || MINIX
  socklen_t c_addr_l;
#elif WINDOWS
  int c_addr_l;
#endif
  client_id();
  char *client_name();
};


class srv_ioid
{
#if WINDOWS
   SOCKET udpSocket;
#elif LINUX || MINIX
   int udpSocket;
#endif
public:
   srv_ioid();
   ~srv_ioid();
   int srv_open();
   void srv_close();
   
   int read(char *buf, int mlen, client_id *client);
   int write(char *buf, int len, client_id *client);
};


int io_init(int argc, char **argv);
int io_close();
void save_pos(poskas pk, int io_id);







#endif
