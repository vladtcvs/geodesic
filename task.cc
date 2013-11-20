#include "geod.h"
#include "start.h"

#include "message.h"

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
int maxcnt=10;

start_data *srv_get_start()
{
  
  
  if (cnt >= maxcnt)
  {
    start_data *ans = NULL;
    
    
    return ans;
  }
  start_data *ans;
  
  ans = new start_data;
  poskas pk(4);
  
  double Rs = 1;
  double R = (1.49+cnt*0.01)*Rs;
  double PI = 3.1415926535897932384626433832795;
  double K = 1;
  double v = 0.99;
  
  
  pk.p[0] = 0;
  pk.p[1] = R;
  pk.p[2] = PI/2;
  pk.v[0] = K;
  pk.v[3] = v*K*sqrt((R-Rs)/R)/R;

  
  ans->pk = pk;
  ans->calc_id = cnt;
  ans->N = 1000;
  ans->h = 30;
  ans->dh = 1e-2;
  
  cnt++;
  
  return ans;
}


void start_data::close()
{
  id.io_close();
}
