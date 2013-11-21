#include "geod.h"
#include "ioid.h"
#include "start.h"

#if LINUX
#include <pthread.h>
#endif


#include "client.h"
#include "server.h"
#include <iostream>
#include <string.h>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;



int main(int argc, char **argv)
{
  
  int i;
  
  
#if WINDOWS
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult != 0) 
  {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
  }
#endif
  
  
  recv_server(NULL);
}
