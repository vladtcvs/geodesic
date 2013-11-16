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

#elif LINUX
   int udpSocket;
   
#endif
   struct sockaddr_in srv_addr;
   
   public:
      ioid();
      ~ioid();
      void setlen(int l);
      void io_open();
      void io_close();
      int write_poskas(poskas pk, int calc_id);
      int read_start(double *buf, int len);
      void fin(int calc_id);
      void getnew();
};


int io_init(int argc, char **argv);
int io_close();
void save_pos(poskas pk, int io_id);

void* recv_server(void* data);
double *srv_get_start(int *calc_id);

#endif