#include "geod.h"
#include "message.h"

#include <string.h>

Msgtype msg::mtype()
{
  return type;
}

msg::msg()
{
  type = GD_NONE;
}



msg_poskas::msg_poskas()
{
  type = GD_POSKAS;
}


msg_start::msg_start()
{
  type = GD_START;
}

msg_fin::msg_fin()
{
  type = GD_FIN;
}

msg_getnew::msg_getnew()
{
  type = GD_GETNEW;
}



static double c2d_8(char *buf)
{
  double a = 0;
  memcpy(&a, buf, 8);
  return a;
}

static int c2i_4(char *buf)
{
  int a = 0;
  memcpy(&a, buf, 4);
  return a;
}

void i2c_4(char *buf, int i)
{
  memcpy(buf, &i, 4);  
}

void d2c_8(char *buf, double a)
{
  memcpy(buf, &a, 8);
}


/* 
 * GETNEW: 	type 	1 byte
 * 		
 * FIN:		type	1 byte
 * 		calc_id	4 byte
 * 
 * POSKAS:	type 	1 byte
 *		calc_id 4 byte
 * 		dim	1 byte
 *		p[0]	8 byte 
 * 		v[0]	8 byte
 * 		p[1]	8 byte
 *		v[1]	8 byte 
 *		.... 
 * START:	type	1 byte
 * 		calc_id	4 byte
 * 		dim	1 byte
 * 		p[0]	8 byte 
 * 		v[0]	8 byte
 * 		p[1]	8 byte
 *		v[1]	8 byte 
 * 		....
 * 		N	4 byte
 * 		h	8 byte
 * 		dh	8 byte
*/

msg* decode(char *buf, int blen)
{
  int i, L;
  int ind;
  msg *ans = NULL;
  msg_fin *mf = NULL;
  msg_poskas *mp = NULL;
  msg_start *ms = NULL;
  msg_signal *mss = NULL;
  if (blen < 1)
    return NULL;
  
  ind = 1;
  
  switch (buf[0])
  {
    case GD_NONE:
      break;
    case GD_GETNEW:
      ans = new msg_getnew;
      break;
    case GD_SIGNAL:
      mss = new msg_signal;
      mss->sig = (Sigtype)buf[ind];
      ind++;
      ans = mss;
      break;
    case GD_FIN:
      mf = new msg_fin;
      mf->calc_id = c2i_4(&(buf[ind]));
      ind++;
      ans = mf;
      break;
    case GD_POSKAS:
      mp = new msg_poskas;
      mp->calc_id = c2i_4(&(buf[ind]));
      ind += 4;
      L = mp->dim = buf[ind];
      ind++;
      
      mp->pk.p.alloc(L);
      mp->pk.v.alloc(L);
      for (i = 0; i < L; i++)
      {
	mp->pk.p[i] = c2d_8(&(buf[ind]));
	ind += 8;
	mp->pk.v[i] = c2d_8(&(buf[ind]));
	ind += 8;
      } 
      ans = mp;
      break;
    case GD_START:
      ms = new msg_start;
      ms->calc_id = c2i_4(&(buf[ind]));
      ind += 4;
      L = ms->dim = buf[ind];
      ind++;
      ms->pk.p.alloc(L);
      ms->pk.v.alloc(L);
      for (i = 0; i < L; i++)
      {
	ms->pk.p[i] = c2d_8(&(buf[ind]));
	ind += 8;
	ms->pk.v[i] = c2d_8(&(buf[ind]));
	ind += 8;
      } 
      ms->N = c2i_4(&(buf[ind]));
      ind += 4;
      ms->h = c2d_8(&(buf[ind]));
      ind += 8;
      ms->dh = c2d_8(&(buf[ind]));
      ind += 8;
      ans = ms;
      break;
  }
  return ans;
}


/* 
 * GETNEW: 	type 	1 byte
 * 		
 * FIN:		type	1 byte
 * 		calc_id	4 byte
 * 
 * POSKAS:	type 	1 byte
 *		calc_id 4 byte
 * 		dim	1 byte
 *		p[0]	8 byte 
 * 		v[0]	8 byte
 * 		p[1]	8 byte
 *		v[1]	8 byte 
 *		.... 
 * START:	type	1 byte
 * 		calc_id	4 byte
 * 		dim	1 byte
 * 		p[0]	8 byte 
 * 		v[0]	8 byte
 * 		p[1]	8 byte
 *		v[1]	8 byte 
 * 		....
 * 		N	4 byte
 * 		h	8 byte
 * 		dh	8 byte
*/
int encode(char *buf, int blen, msg *message)
{
  msg_fin* mf = NULL;
  msg_poskas* mp = NULL;
  msg_start *ms = NULL;
  
  int L, i;
  if (blen < 1)
    return -1;
  int ind = 0;
  switch(message->mtype())
  {
    case GD_GETNEW:
      buf[ind] = GD_GETNEW;
      ind++;
      break;
    case GD_SIGNAL:
      buf[ind] = GD_SIGNAL;
      ind++;
      buf[ind] = ((msg_signal*)message)->sig;
      ind++;
      break;
    case GD_FIN:
      if (blen < 5)
	return -1;
      mf = (msg_fin*)message;
      buf[ind] = GD_FIN;
      ind++;
      i2c_4(&(buf[ind]), mf->calc_id);
      break;
    case GD_POSKAS:
      mp = (msg_poskas*)message;
      L = mp->dim;
      if (blen < 2*8*L + 4+1)
	return -1;
      buf[ind] = GD_POSKAS;
      ind++;
      i2c_4(&(buf[ind]), mp->calc_id);
      ind += 4;
      buf[ind] = mp->dim;
      ind++;
      for (i = 0; i < L; i++)
      {
	d2c_8(&(buf[ind]), (double)(mp->pk.p[i]));
	ind+=8;
	d2c_8(&(buf[ind]), (double)(mp->pk.v[i]));
	ind+=8;
      }
      break;
    case GD_START:
      ms = (msg_start*)message;
      L = ms->dim;
      if (blen < 2*8*L + 4+1)
	return -1;
      buf[ind] = GD_START;
      ind++;
      i2c_4(&(buf[ind]), ms->calc_id);
      ind += 4;
      buf[ind] = ms->dim;
      ind++;
      for (i = 0; i < L; i++)
      {
	d2c_8(&(buf[ind]), ms->pk.p[i]);
	ind+=8;
	d2c_8(&(buf[ind]), ms->pk.v[i]);
	ind+=8;
      }
      i2c_4(&(buf[ind]), ms->N);
      ind+=4;
      d2c_8(&(buf[ind]), ms->h);
      ind+=8;
      d2c_8(&(buf[ind]), ms->dh);
      ind+=8;
      break;
    case GD_NONE:
    default:
      break;
  }
  return ind;
}

msg_signal::msg_signal()
{
  type = GD_SIGNAL;
  sig = GD_S_NONE;
}
