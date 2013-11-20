#ifndef __IOID_H__
#define __IOID_H__

#include "geod.h"
#ifdef WINDOWS
#include <winsock.h>
#elif LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class ioid
{
   std::vector<double> msgbuf;
#ifdef WINDOWS
   SOCKET udpSocket;
   int tid;
#elif LINUX
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
#ifdef LINUX
  socklen_t c_addr_l;
#elif WINDOWS
  int c_addr_l;
#endif
  client_id();
  char *client_name();
};


class srv_ioid
{
#ifdef WINDOWS
   SOCKET udpSocket;
#elif LINUX
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

void* recv_server(void* data);





#endif