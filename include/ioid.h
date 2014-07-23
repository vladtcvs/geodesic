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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <geod.h>
#include <tensor.h>

class ioid
{
   std::vector<double> msgbuf;
   int udpSocket;   
   pid_t tid;
   
   struct sockaddr_in srv_addr;
   
   public:
      ioid();
      ~ioid();
      void setlen(int l);
      void io_open(const char* server_ip);
      void io_close();
      int write(char *buf, int len);
      int read(char *buf, int len);
};


struct client_id
{
  sockaddr_in c_addr;
  socklen_t c_addr_l;
  client_id();
  char *client_name();
};


class srv_ioid
{
   int udpSocket;
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





extern char my_ip[20];



#endif
