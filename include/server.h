/** File server.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#ifndef SERVER_H
#define SERVER_H

#include "start.h"

struct sd_stel
{
  start_data *data;
  time_t ready;
  sd_stel();
  sd_stel(start_data *d);
};

void* recv_server(void* data);

#define CALC_TIMEOUT 600000

#endif
