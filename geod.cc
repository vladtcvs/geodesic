#include "geod.h"
#include <unistd.h>
#include <glib.h>

#include <iostream>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;


static inline poskas geodesic_step(poskas pos, real dh, real h1)
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


static inline poskas geodesic(start_data *sd)
{
  poskas pk = sd->pk;
  real h = 0;
  int i = 0;
  int j;
  int d=pk.p.dim();
  
  for (i = 0; i < sd->N; i++)
  {
    if (sd->id.write_poskas(pk,sd->calc_id) == -1) 
      return pk;
    pk=geodesic_step(pk,sd->dh,sd->h/sd->N);
  }
  sd->id.write_poskas(pk,sd->calc_id);
  sd->id.fin();
  return pk;
}



gpointer geodesic_glib(gpointer data)
{
  start_data *sd;
  
  while (1)
  {
    sd = get_start();
    if (sd == NULL)
    {
      break;
    }
    geodesic(sd);
    sd->close();
  }
  return NULL;
}




typedef GThread * pgthread;

int main(int argc, char **argv)
{
  
  int i;
  int server = 0;
  
  if (argc > 1)
  {
    if (strcmp(argv[1],"server")==0)
      server = 1;
  } 
  if (server)
  {
    int L = 4;
    recv_server(&L);
  }
  else
  {
    start_data sd;
    
    int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
  
  
    int nthr = numCPU;
    pgthread *gth = new pgthread[nthr];
  
    std::cout<<"Machine with "<<numCPU<<" CPU\n";
    std::cout<<"\n";
    
    if (argc > 1)
      strcpy(server_ip, argv[1]);
    
    
    io_init(argc, argv);
 
    for (i = 0; i < nthr; i++)
    {
      char tname[10];
      sprintf(tname, "thread%05i", i);
      gth[i] = g_thread_new(tname, geodesic_glib, NULL);
      // ждем, чтобы поток успел скопировать данные
      usleep(100000);
    }
  
  
  
    // ждем окончания всех потоков
    for (i = 0; i < nthr; i++)
      g_thread_join(gth[i]);
  
  
    io_close();
  
    delete gth;
  }
  return 0;
}
