#include "geod.h"

#include "ioid.h"


#include <string.h>
#include "message.h"
#include "task.h"

/**
 * Эта функция запускается только на сервере, она сохраняет принятые от клиентов 
 * данные 
 */
static void save_pos(FILE *outf, poskas pk, int calc_id)
{
    int i, L = pk.p.dim();
    PRINT_LOG
    //printf("id = %i L = %i ", calc_id, L);
    fprintf(outf, "%i ", calc_id);
    for (i = 0; i < L; i++)
      fprintf(outf, "%lf ", (double)(pk.p[i]));
    fprintf(outf,"\n");
    fflush(outf);
}




/** 
 * Эта функция запускается на сервере. 
 * Она ожидает поступающих от клиентов результатов расчетов.
 */
void* recv_server(void* data)
{
  int L = *((int*)data);
  int len = L*2+2;
  char buf[1000];
  
  srv_ioid io;
  client_id cl;
  
  poskas pk;
  int calc_id;
  
//  int i;
#ifdef LINUX  
  FILE *outf = fopen("out", "wt");
#elif WINDOWS
  FILE *outf = NULL;
  errno_t errnum;
  if (errnum = fopen_s(&outf, "out", "wt") < 0)
  {
	  printf_s("error %i\n", errnum);
	  return NULL;
  }
#endif
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
			
			save_pos(outf, pk, calc_id);
	      }
	      break;
	    case GD_GETNEW:
	      {
		printf("recv GETNEW from %s\n", cl.client_name());
		
		//int res;
		msg *start = srv_get_start();
		if (start->mtype()==GD_START)
		{		 
		  char mess[1000];
		 
		  
		  dlen = encode(mess, 1000, start);
		  PRINT_LOG
		  
		  io.write(mess, dlen, &cl);
		  
		  delete start;
		}
		else
		{
		  msg_signal *mm = new msg_signal;
		  mm->sig = GD_S_END;
		  char mess[1000];
		  dlen = encode(mess, 1000, mm);
		  io.write(mess, dlen, &cl);
		  delete mm;
		}
	      }
	      break;
	    case GD_FIN:
	      {
			PRINT_LOG
			printf("recv FIN from %s calc_id %i\n", cl.client_name(), ((msg_fin*)rec)->calc_id);
 			
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


