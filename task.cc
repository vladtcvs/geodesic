#include "geod.h"


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

start_data get_start()
{
  start_data sd;
  poskas pk(4);
  ioid id;
  char fname[10];
  
  sprintf(fname, "out%05i", cnt);
  id.out = fopen(fname, "wt");
  
  
  real Rs = 1;
  real R=(1.4999+cnt*0.0002)*Rs;
  real h1=30;
  real dh=1e-2;
  real K = 1;
  real v = 1;
  
  
  pk.p[0] = 0;
  pk.p[1] = R;
  pk.p[2] = PI/2;
  pk.v[0] = K;
  pk.v[3] = v*K*sqrt((R-Rs)/R)/R;

  
  sd.pk = pk;
  sd.N = 1000;
  sd.h = h1;
  sd.dh = dh;
  sd.id = id;
  
  cnt++;
  
  return sd;
}

