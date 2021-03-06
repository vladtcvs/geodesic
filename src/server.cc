/** File server.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#include "geod.h"

#include "ioid.h"
#include <iostream>

#include <string.h>
#include "message.h"
#include "task.h"

#include <list>

#include <time.h>
#include "server.h"
#include "eye.h"

#include "save.h"

sd_stel::sd_stel()
{
  ready = time(NULL);
  data = NULL;
}

sd_stel::sd_stel(start_data* d)
{
  ready = time(NULL);
  data = d;
}



/** 
 * Эта функция запускается на сервере. 
 * Она ожидает поступающих от клиентов результатов расчетов.
 */
void* recv_server(void* data)
{
  int L;
  int len = 1000;
  char buf[1000];
  
//  eye observer = get_observer();
  
  
  std::list<sd_stel> sval;
  
  
  srv_ioid io;
  client_id cl;
  
  poskas pk;
  int calc_id;
  
//  int i;
  FILE *outf = fopen("out", "wt");
  if (outf < 0)
  {
    PRINT_LOG
    return NULL;
  }
  
  PRINT_LOG
  
  if (io.srv_open() < 0)
    return NULL;
  
  int go = 1;
  
  int dlen = -1;
  while (go)
  {
//	  int i;
	  
	  
	  int bytesrecv = io.read(buf, 1000, &cl);
	  
	  if (bytesrecv == -1)
	    break;
	  msg *rec = decode(buf, bytesrecv);
	  
	  Msgtype msg_type = rec->mtype();
	    
	  switch(msg_type)
	  {
	    case GD_POSKAS:
	      {
			msg_poskas *pos = (msg_poskas*)rec;
			calc_id = pos->calc_id;
			pk=pos->pk;
			
			
			
			PRINT_LOG
			use_result(pk, outf, calc_id);		
	      }
	      break;
	    case GD_GETNEW:
	      {
		std::cout<<"recv GETNEW from "<<cl.client_name()<<" tid = "<<rec->thread;
		
		
		std::list<sd_stel>::iterator it;
				
		//int res;
		start_data *start;
		
		start = srv_get_start();
		
		if (start != NULL)
		{		 
		  char mess[1000];
		  msg_start *ms = start2msg(start);
		  ms->thread = rec->thread;
		  dlen = encode(mess, 1000, ms);
		  PRINT_LOG
		  
		  io.write(mess, dlen, &cl);
		  std::cout<<" calc_id = "<<start->calc_id<<"\n";
		  
		}
		else
		{
		  printf("\n");
		  msg_signal *mm = new msg_signal;
		  mm->sig = GD_S_END;
		  mm->thread = rec->thread;
		  char mess[1000];
		  dlen = encode(mess, 1000, mm);
		  io.write(mess, dlen, &cl);
		  delete mm;
		}
	      }
	      break;
	    case GD_FIN:
	      {
		msg_fin *mf = (msg_fin*)rec;
		PRINT_LOG
		std::cout<<"recv FIN from "<<cl.client_name()<<" tid = "<<mf->thread<<" calc id= "<<mf->calc_id<<"\n";
 			
			
	      }
	      break;
	    default:
	      break;
	  }
	  delete rec;
  }
  
  io.srv_close();
  
  
  
  fclose(outf);
  
  return NULL;
}


