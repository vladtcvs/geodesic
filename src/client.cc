/** File client.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#include <iostream>
#include <pthread.h>



#include <geod.h>
#include <start.h>
#include <message.h>
#include <runge.h>
#include <space.h>



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

  solver *intgr = new solver_rk4;  

  diff_fun *df = new diff_fun_space;
  
  int d=pk.p.dim();
  
  msg_poskas *mess = new msg_poskas;
  char buf[1000];
  int len;
  


  mess->calc_id = sd->calc_id;
  mess->dim = d;
    
  intgr->init(df, pk, sd->dh);
  real len0 = sd->h / sd->N;
	

  for (i = 0; i < sd->N; i++)
  {
    mess->pk = pk;
    len = encode(buf, 1000, mess);
    if (sd->id.write(buf,len) == -1) 
      break;
    
     
    pk=intgr->makestep(len0);
    
    if (check_pk(pk)==0)
      break;
  }
  
  delete intgr; 
  delete mess;
  delete df;
  
  return pk;
}





start_data *get_start(int64_t tid, ioid id)
{
  start_data* sd;
  
  
  
  char buf[1000];
  
  
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


void* geodesic_pthread(void* data)
{
  start_data *sd;
  int64_t tid;
  char *s_ip = (char*)data;
  
  ioid id;
  id.io_open(s_ip);
  
  
  tid = pthread_self();
  
  bool go = true;
  
  while (go)
  {
    try
    {
      while (1)
      {
	sd = get_start(tid, id);
	if (sd == NULL)
	{	
	  PRINT_LOG
	  go = false;
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
	
      }
    }
    catch(int err)
    {
      std::cout<<"exeption "<<error_str(err)<<"\n";
      std::cout<<"sending FIN\n";
      msg_fin *mf = new msg_fin;
      mf->calc_id = sd->calc_id;
      mf->thread = tid;
      char buf[1000];
      int len = encode(buf, 1000, mf);
      sd->id.write(buf,len);
      delete mf;
      
    }
  }
  id.io_close();
  return NULL;
}

