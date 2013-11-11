#include "geod.h"
#include <unistd.h>
#include <glib.h>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;








poskas geodesic_step(poskas pos, real dh, real h1)
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


poskas geodesic(poskas pos, real dh, real h1, int N, ioid id)
{
  poskas pk = pos;
  real h = 0;
  int i = 0;
  int j;
  int d=pk.p.dim();
  
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < d; j++)
      fprintf(id.out, "%lf ", (double)pk.p[j]);
    fprintf(id.out, "\n");
    pk=geodesic_step(pk,dh,h1/N);
  }
  for (j = 0; j < d; j++)
      fprintf(id.out, "%lf ", (double)pk.p[j]);
  fprintf(id.out, "\n");

  
  return pk;
}



gpointer geodesic_glib(gpointer data)
{
  start_data *sd = (start_data*)data;
  poskas pk = sd->pk;
  real h1 = sd->h;
  real dh = sd->dh;
  int N = sd->N;
  ioid id = sd->id;
  geodesic(pk, dh, h1, N, id);
  
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
    sd = get_start();
    gth[i] = g_thread_new(tname, geodesic_glib, (gpointer)(&sd));
    // ждем, чтобы поток успел скопировать данные
    sleep(1);
  }
  
  
  
  // ждем окончания всех потоков
  for (i = 0; i < nthr; i++)
    g_thread_join(gth[i]);
  
  delete gth;
  
  return 0;
}
