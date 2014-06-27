/** File geod.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#include "geod.h"
#include "ioid.h"
#include "start.h"

#include <pthread.h>


#include "client.h"
#include "server.h"
#include <iostream>
#include <string.h>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;



int main(int argc, char **argv)
{
  try
  {
  int i;
 
  
    start_data sd;
    int numCPU = 2;
    
    
    numCPU = sysconf(_SC_NPROCESSORS_ONLN);;
  
    int nthr = numCPU;
    pthread_t* pth = new pthread_t[nthr];


    std::cout<<"Machine with "<<numCPU<<" CPU\n";
    std::cout<<"\n";
    
    if (argc > 1)
    {
      strcpy(server_ip, argv[1]);
    }
    
    for (i = 0; i < nthr; i++)
    {
      
	  int res = pthread_create(&(pth[i]), NULL, geodesic_pthread, NULL);
	  // ждем, чтобы поток успел скопировать данные
	  usleep(100000);
     }
  
      // ждем окончания всех потоков
 
    for (i = 0; i < nthr; i++)
      pthread_join(pth[i], NULL);
  
    
    delete[] pth;
  }
  catch(int err)
  {
    std::cout<<"exception "<<err<<"\n";
    
  }
  return 0;
}

