/** File task.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#include <stdlib.h>
#include <list>



#include <geod.h>
#include <start.h>
#include <emit.h>
#include <eye.h>
#include <save.h>


const real PI = 3.1415926535897932384626433832795;

static inline real sqr(real x)
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

static std::list<light_src> pnt;


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
  
    Lvector pos(DIM), dir(DIM);
    pos[0] = 0;
    pos[1] = R;
    pos[2] = PI/2;
    pos[3] = 0;
  
    dir[1] = 1;
    dir[2] = 0;
    dir[3] = 1;
  
    
    if (cnt >= maxcnt)
    {
      ans = NULL;
      return ans;
    }
    pk = emit_object_vel(pos, v, dir);
    ans->pk = pk;
    ans->calc_id = cnt;
    ans->N = 100;
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
    dir.alloc(DIM);
    
    do
    {
      dir[1] = (((double)rand())/RAND_MAX)*2-1;
      //dir[2] = (((double)rand())/RAND_MAX)*2-1;
      dir[2] = 0;
      dir[3] = (((double)rand())/RAND_MAX)*2-1;
    }
    while (dir[1] == 0 && dir[2] == 0 && dir[3] == 0);
    int i;
    real s = 0;
    for (i = 1; i < DIM; i++)
      s += sqr(dir[i]);
    s = sqrt(s);
    dir /= s;
    pk=emit_light(pk.p, dir);
    ans->pk = pk;
    ans->calc_id = lcnt;
    ans->N = 100;
    ans->h = 30;
    ans->dh = 1e-2;
    lcnt--;
  }
 
  return ans;
}

void push_pos(poskas pk)
{
  light_src lsr={pk,20};
  pnt.push_back(lsr);
}

void use_result(poskas pk, FILE *outf, int calc_id)
{
	if (calc_id >= 0)
        {
		PRINT_LOG
                push_pos(pk);
        	save_pos(outf, pk, calc_id);
        }
            
        else
        {
        	//if (observer.if_in_eye(pk))
                save_pos(outf, pk, calc_id);
                //obspnt op = observer.observe(pk);
                //if (op.u)
                //{
               		//  draw_point(outf, op);
                 //}
        } 

}






eye get_observer()
{
  eye obs;
  
  Lvector npos, ndir, ntop;
  Lvector ndp;
  
  real t, r, th, fi;
  
  t = 6;
  r = 6;
  th = PI/2;
  fi = PI/2;
  
  
  
  npos.alloc(4);
  ndir.alloc(4);
  ntop.alloc(4);
  ndp.alloc(4);
  
  PRINT_LOG;
  
  npos[0] = t;
  npos[1] = r;
  npos[2] = th;
  npos[3] = fi;
  
  ntop[0] = 0;
  ntop[1] = 0;
  ntop[2] = 1;
  ntop[3] = 0;
  
  ndir[0] = 0;
  ndir[1] = -1;
  ndir[2] = 0;
  ndir[3] = 0;
  
  ndp[0] = 10;
  ndp[1] = 10;
  ndp[2] = 0.1;
  ndp[3] = 100;
  
  
  
  obs.init(npos, ntop, ndir, ndp);
  PRINT_LOG;
  return obs;
}


