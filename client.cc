#include "geod.h"
#include "start.h"
#include "message.h"

#include <iostream>

#ifdef LINUX
#include <pthread.h>
#endif

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

static int check_pk(poskas pk)
{
  int i, L = pk.p.dim();
  int f = 1;
  for (i = 0; f && i < L; i++)
  {
    f *= (fabs(pk.p[i]) < 1e20);
    f *= (fabs(pk.v[i]) < 1e20);
  }
  return f;
}

static inline poskas geodesic(start_data *sd)
{
  poskas pk = sd->pk;
  real h = 0;
  int i = 0;
 
  int d=pk.p.dim();
  
  msg_poskas *mess = new msg_poskas;
  char buf[1000];
  int len;
  
  mess->calc_id = sd->calc_id;
  mess->dim = d;
    
  
  for (i = 0; i < sd->N; i++)
  {
    mess->pk = pk;
    len = encode(buf, 1000, mess);
    if (sd->id.write(buf,len) == -1) 
      break;
    
    pk=geodesic_step(pk,sd->dh,sd->h/sd->N);
    
    if (check_pk(pk)==0)
      break;
  }
  
  delete mess;
  
  
  return pk;
}





start_data *get_start(int64_t tid)
{
  start_data* sd;
  
  ioid id;
  
  char buf[1000];
  
  id.io_open();
  
  msg *mg = new msg_getnew;
 
  mg->thread = tid;
  
  std::cout<<"My tid = "<<tid<<"\n";
  
  int len = encode(buf, 1000, mg);
  
  id.write(buf, len);
  
  int cnt;
  
  msg *m; 
  do
  {
    cnt = id.read(buf, 1000);
    if (cnt < 0)
      return NULL;
  
    m = decode(buf, cnt);
  }
  while (m->mtype()  == GD_SIGNAL && ((msg_signal*)m)->sig==GD_S_WAIT);
    
  
  if (m->mtype() != GD_START)   
    return NULL;
  
  if (m->thread - tid != 0)
  {
    std::cout<<"Wrong thread"<<m->thread<<"! \n";
    return NULL;
  }
  
  msg_start *mm = (msg_start*)m;
  sd = msg2start(mm, id);
  
  
  for (int i =0; i < sd->pk.p.dim(); i++)
  {
    printf("%lf ", (double)(sd->pk.p[i]));
  }
  printf("\n");
  printf("N = %i h = %lf dh = %lf  ID = %i\n", sd->N, (double)(sd->h), (double)(sd->dh), sd->calc_id);
  
  delete m;
  
  return sd;
}


#ifdef LINUX
void* geodesic_pthread(void* data)
#elif WINDOWS
DWORD WINAPI geodesic_winthreads( LPVOID data )
#endif
{
  start_data *sd;
  int64_t tid;
#ifdef LINUX
  tid = pthread_self();
#elif WINDOWS
  tid = GetCurrentThreadId();
#endif
  
  while (1)
  {
    sd = get_start(tid);
    if (sd == NULL)
    {
      PRINT_LOG
      break;
    }
    PRINT_LOG
    
    geodesic(sd);
    msg_fin *mf = new msg_fin;
    mf->calc_id = sd->calc_id;
    mf->thread = tid;
    char buf[1000];
    printf("FIN: calc_id = %i\n", mf->calc_id);
    int len = encode(buf, 1000, mf);
    sd->id.write(buf,len);
    delete mf;
  
    
    sd->close();
  }
#ifdef LINUX
  return NULL;
#elif WINDOWS
  return 0;
#endif
}
