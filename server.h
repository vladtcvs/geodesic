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
