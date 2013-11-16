#include "geod.h"
#include "start.h"

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

double *srv_get_start(int *calc_id)
{
  double *ans;
  
  if (cnt >= maxcnt)
  {
    ans = new double[1];
    *ans = 0;
    return ans;
  }
  
  
  ans = new double[14];
  poskas pk(4);
  
  double Rs = 1;
  double R = (1.49+cnt*0.01)*Rs;
  double PI = 3.1415926535897932384626433832795;
  double K = 1;
  double v = 0.7;
  
  
  pk.p[0] = 0;
  pk.p[1] = R;
  pk.p[2] = PI/2;
  pk.v[0] = K;
  pk.v[3] = v*K*sqrt((R-Rs)/R)/R;

  ans[0] = 13;
  ans[1] = 4;
  ans[2] = pk.p[0];
  ans[3] = pk.p[1];
  ans[4] = pk.p[2];
  ans[5] = pk.p[3];
  ans[6] = pk.v[0];
  ans[7] = pk.v[1];
  ans[8] = pk.v[2];
  ans[9] = pk.v[3];
  
  ans[10] = 1000;  // N
  ans[11] = 600;   // h1
  ans[12] = 1e-1; // dh
  
  ans[13] = cnt;   // calc id
  *calc_id = cnt;
  cnt++;
  
  return ans;
}


void start_data::close()
{
  id.io_close();
}
