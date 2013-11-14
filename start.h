#ifndef __START_H__
#define __START_H__

#include "ioid.h"


struct start_data
{
  poskas pk;
  real h;
  real dh;
  int N;
  ioid id;
  int calc_id;
  void close();
};

start_data* get_start(void);

#endif