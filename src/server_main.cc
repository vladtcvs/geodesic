/** File server_main.cc author Vladislav Tcendrovskii
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

#if LINUX
#include <pthread.h>
#endif

#include "errors.h"

#include "server.h"
#include <iostream>
#include <string.h>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;



int main(int argc, char **argv)
{
  
  int i;
  
  
  try
  {
  
    recv_server(NULL);
  }
  catch(int errnum)
  {
    std::cerr<<error_str(errnum)<<"\n";
    
  }
}
