/** File task.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#include "geod.h"
#include "start.h"

#include "message.h"
#include "emit.h"
#include <stdlib.h>
#include <list>

const real PI = 3.1415926535897932384626433832795;

static real sqr(real x)
{
  return x*x;
}

tensor2 Metric(Lvector p)
{
	double Rs=1, R, O;
	int L = p.dim();
	tensor2 res(L);
	int i, j;
	if (L != 4)
	    throw 2;
	
	R = p[1];
	O = p[2];
	
	
	for (i = 0; i < L; i++)
	for (j = 0; j < L; j++)
	{	
		res[i][j] = 0;
	}
	
	res[0][0] = 1-Rs/R;
	res[1][1] = -1/(1-Rs/R);
	res[2][2] = -sqr(R);
	res[3][3] = -sqr(R*sin(O));
	
	return res;
}





int cnt = 0;
int maxcnt=1;
int lcnt = -1;

struct light_src
{
  poskas pk;
  int nl;
};

std::list<light_src> pnt;


start_data *srv_get_start()
{
  
  
  
  start_data *ans;
  
  ans = new start_data;
  
  
  poskas pk;
  
  if (pnt.size() == 0)
  {
    
    double Rs = 1;
    double R = (3+cnt*0.1)*Rs;
    double PI = 3.1415926535897932384626433832795;
    double K = 1;
    double v = 0.5;
  
    Lvector pos(4), dir(4);
    pos[0] = 0;
    pos[1] = R;
    pos[2] = PI/2;
    pos[3] = 0;
  
    dir[1] = 1;
    dir[2] = 0;
    dir[3] = 1;
  
    
    if (cnt >= maxcnt)
    {
      start_data *ans = NULL;
    
    
      return ans;
    }
    pk = emit_object_vel(pos, v, dir);
    ans->pk = pk;
    ans->calc_id = cnt;
    ans->N = 50;
    ans->h = 30;
    ans->dh = 1e-2;
    cnt++;
  }
  else
  {
    light_src lsr = pnt.front();
    
    pnt.pop_front();
    
    pk = lsr.pk;
    lsr.nl--;
    if (lsr.nl >= 1)
      pnt.push_front(lsr);
    
    Lvector dir;
    dir.alloc(4);
    
    do
    {
      dir[1] = (((double)rand())/RAND_MAX)*2-1;
      dir[2] = (((double)rand())/RAND_MAX)*2-1;
      dir[3] = (((double)rand())/RAND_MAX)*2-1;
    }
    while (dir[1] == 0 && dir[2] == 0 && dir[3] == 0);
    int i;
    real s = 0;
    for (i = 1; i < 4; i++)
      s += sqr(dir[i]);
    s = sqrt(s);
    dir /= s;
    pk=emit_object_vel(pk.p, 1, dir);
    ans->pk = pk;
    ans->calc_id = lcnt;
    ans->N = 100;
    ans->h = 30;
    ans->dh = 1e-2;
    lcnt--;
  }
  /*
   * 
  pk.p[0] = 0;
  pk.p[1] = R;
  pk.p[2] = PI/2;
  pk.v[0] = K;
  pk.v[3] = v*K*sqrt((R-Rs)/R)/R;
*/
  
  
  
  
  return ans;
}



void push_pos(poskas pk)
{
  light_src lsr={pk,20};
  pnt.push_back(lsr);
}
