#include "geod.h"
#include "start.h"
#include "message.h"

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
 
  int d=pk.p.dim();
  
  for (i = 0; i < sd->N; i++)
  {
    if (sd->id.write_poskas(pk,sd->calc_id) == -1) 
    {
      sd->id.fin(sd->calc_id);
      return pk;
    }
    pk=geodesic_step(pk,sd->dh,sd->h/sd->N);
  }
  sd->id.write_poskas(pk,sd->calc_id);
  sd->id.fin(sd->calc_id);
  return pk;
}





start_data *get_start(void)
{
  start_data* sd;
  
  ioid id;
  
  double buf[1000];
  
  id.io_open();
  id.getnew();
  if (id.read_start(buf, 1000) < 0)
    return NULL;
  
  int msglen = (int)buf[0];
  if (msglen == 0)
    return NULL;
  
  if (msglen < 13)
    return NULL;
  
  int dim = (int)buf[1];
  
  poskas pk(dim);
  id.setlen(dim);
  
  pk.p[0] = buf[2];
  pk.p[1] = buf[3];
  pk.p[2] = buf[4];
  pk.p[3] = buf[5];
  pk.v[0] = buf[6];
  pk.v[1] = buf[7];
  pk.v[2] = buf[8];
  pk.v[3] = buf[9];
  
  sd = new start_data;
  
  sd->pk = pk;
  sd->N = (int)buf[10];
  sd->h = buf[11];
  sd->dh = buf[12];
  sd->id = id;
  
  sd->calc_id = (int)buf[13];
  
  
  return sd;
}


#ifdef LINUX
gpointer geodesic_glib(gpointer data)
#elif WINDOWS
DWORD WINAPI geodesic_winthreads( LPVOID data )
#endif
{
  start_data *sd;
  
  while (1)
  {
    sd = get_start();
    if (sd == NULL)
    {
      PRINT_LOG
      break;
    }
    PRINT_LOG
    
    geodesic(sd);
    sd->close();
  }
#ifdef LINUX
  return NULL;
#elif WINDOWS
  return 0;
#endif
}
