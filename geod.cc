#include "geod.h"

//#include <glib.h>

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
/*
gpointer geodesic_glib(gpointer data)
{
  start_data *sd = data;
  poskas pk = sd->pk;
  real h1 = sd->h;
  real dh = sd->dh;
  int N = sd->N;
  FILE *out = pk.out;
  geodesic(pk, dh, h1, N, out);
  
  return NULL;
}
*/


int main()
{
  poskas pk(4);
  real h;
  real h1=30;
  real dh=1e-2;
  FILE *out = fopen("out", "wt");
  
  real Rs = 1;
  real R = 1.4999*Rs;
  real K = 1;
  real v = 1;
  
  start_data sd;
  
  
  
  pk.p[0] = 0;
  pk.p[1] = R;
  pk.p[2] = PI/2;
  pk.v[0] = K;
  pk.v[3] = v*K*sqrt((R-Rs)/R)/R;

  //g_thread_init(NULL);
  sd.pk = pk;
  sd.N = 1000;
  sd.h = h1;
  sd.dh = dh;
  sd.out = out;
  //GThread *thread_id = g_thread_create(geodesic_glib, &sd, TRUE, NULL);
  
  geodesic(pk, dh, h1, 1000, out);
  
  fclose(out);
  
  return 0;
}
