#include "geod.h"


static poskas fun(poskas pk)
{
	
	int L = pk.v.dim();
	poskas res(L);
	Lvector dv(L);
	tensor3 Gm = Kristofel(pk.p);
	res.p = pk.v;
	int i, j, k;
	for (i = 0; i < L; i++)
	for (j = 0; j < L; j++)
	for (k = 0; k < L; k++)
		res.v[i] -= Gm[i][j][k]*pk.v[j]*pk.v[k];
	return res;
}
/*
static poskas fun2(poskas pk)
{
	
	int L = pk.v.dim();
	poskas res(L);
	res.p = pk.v;
	double x1 = pk.p[0];
	double x2 = pk.p[1];
	double v1 = pk.v[0];
	double v2 = pk.v[1];
	res.v[0] = x1/(1+x1*x1)*(v2*v2-v1*v1);
	res.v[1] = -2/x1*v1*v2;
	
	return res;
}
*/

poskas runge_kutta4(poskas &pk, real h)
{
	poskas res, k1, k2, k3, k4;
	//PRINT_LOG
	
	k1 = fun(pk)*h;
	k2 = fun(pk + k1/2)*h;
	k3 = fun(pk + k2/2)*h;
	k4 = fun(pk + k3)*h;
	return pk + (k1+k2*2+k3*2+k4)/6;
}
