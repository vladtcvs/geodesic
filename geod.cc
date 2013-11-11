#include "geod.h"
#include <unistd.h>
#include <glib.h>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;

const real PI = 3.1415926535897932384626433832795;






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


poskas geodesic(poskas pos, real dh, real h1, int N, FILE *out)
{
  poskas pk = pos;
  real h = 0;
  int i = 0;
  int j;
  int d=pk.p.dim();
  
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < d; j++)
      fprintf(out, "%lf ", (double)pk.p[j]);
    fprintf(out, "\n");
    pk=geodesic_step(pk,dh,h1/N);
  }
  for (j = 0; j < d; j++)
      fprintf(out, "%lf ", (double)pk.p[j]);
  fprintf(out, "\n");

  
  return pk;
}

struct start_data
{
  poskas pk;
  real h;
  real dh;
  int N;
  FILE *out;
};

gpointer geodesic_glib(gpointer data)
{
  start_data *sd = (start_data*)data;
  poskas pk = sd->pk;
  real h1 = sd->h;
  real dh = sd->dh;
  int N = sd->N;
  FILE *out = sd->out;
  geodesic(pk, dh, h1, N, out);
  
  return NULL;
}


typedef GThread * pgthread;

int main()
{
  poskas pk(4);
  real h;
  real h1=30;
  real dh=1e-2;
  FILE *out1 = fopen("out1", "wt"), *out2 = fopen("out2", "wt");
  
  int i;
  real Rs = 1;
  real R;
  real K = 1;
  real v = 1;
  
  start_data sd;
  
  int nthr = 2;
  pgthread *gth = new pgthread[nthr];
  
  R=1.4999*Rs;
  
  pk.p[0] = 0;
  pk.p[1] = R;
  pk.p[2] = PI/2;
  pk.v[0] = K;
  pk.v[3] = v*K*sqrt((R-Rs)/R)/R;

  
  sd.pk = pk;
  sd.N = 1000;
  sd.h = h1;
  sd.dh = dh;
  sd.out = out1;
  
  
  
  gth[0] = g_thread_new("thread 0", geodesic_glib, (gpointer)(&sd));
  
  // ждем, чтобы поток успел скопировать данные
  sleep(1);
  
  
  R=1.5001*Rs;
  
  pk.p[0] = 0;
  pk.p[1] = R;
  pk.p[2] = PI/2;
  pk.v[0] = K;
  pk.v[3] = v*K*sqrt((R-Rs)/R)/R;

  
  sd.pk = pk;
  sd.N = 1000;
  sd.h = h1;
  sd.dh = dh;
  sd.out = out2;
  
  
  
  gth[1] = g_thread_new("thread 1", geodesic_glib, (gpointer)(&sd));
  
  // ждем окончания всех потоков
  for (i = 0; i < nthr; i++)
    g_thread_join(gth[i]);
  
  delete gth;
  
  fclose(out1);
  fclose(out2);
  return 0;
}
