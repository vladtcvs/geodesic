#include "geod.h"
#include <unistd.h>
#include <glib.h>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;








inline poskas geodesic_step(poskas pos, real dh, real h1)
{
    poskas pk=pos;
    real h = 0;
    do
    {
      pk = runge_kutta4(pk,dh);
      h += dh;
    } 
    while(h <= h1);
  
    return pk;
}


inline poskas geodesic(poskas pos, real dh, real h1, int N, ioid id)
{
  poskas pk = pos;
  real h = 0;
  int i = 0;
  int j;
  int d=pk.p.dim();
  
  for (i = 0; i < N; i++)
  {
    if (id.write_poskas(pk) == -1) 
      return pk;
    pk=geodesic_step(pk,dh,h1/N);
  }
  id.write_poskas(pk);
  
  return pk;
}



gpointer geodesic_glib(gpointer data)
{
  start_data *sd;
  
  while (1)
  {
    sd = get_start();
    if (sd == NULL)
      break;
    geodesic(sd->pk, sd->dh, sd->h, sd->N, sd->id);
   
  }
  return NULL;
}


typedef GThread * pgthread;

int main()
{
  
   int i;
  
  
  start_data sd;
  
  int nthr = 2;
  pgthread *gth = new pgthread[nthr];
  
  
  
  
  for (i = 0; i < nthr; i++)
  {
    char tname[10];
    sprintf(tname, "thread%05i", i);
    gth[i] = g_thread_new(tname, geodesic_glib, NULL);
    // ждем, чтобы поток успел скопировать данные
    sleep(1);
  }
  
  
  
  // ждем окончания всех потоков
  for (i = 0; i < nthr; i++)
    g_thread_join(gth[i]);
  
  delete gth;
  
  return 0;
}
