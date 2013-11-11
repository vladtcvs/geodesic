#include "geod.h"

real a=20;

real sqr(real x)
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


tensor2 dGi(Lvector p, int n)
{
	int L = p.dim();
	tensor2 res(L);
	double coef[5] = {1/(12*H), -2/(3*H), 0, 2/(3*H), -1/(12*H)};
	Lvector  h(L);
	int i;
	for (i = 0; i < L; i++)
	{
		h[i] = (i==n)*H;
	}

	for (i = 0; i < 5; i++)
	{
		Lvector dp = h * (i-2);
		tensor2 metr = Metric(p + dp);
		res += metr*(coef[i]);
	}


	return res;
}


/*
tensor2 dGi(Lvector p, int n)
{
	int L = p.dim();
	tensor2 res(L);
	double coef[5] = {1/(12*H), -2/(3*H), 0, 2/(3*H), -1/(12*H)};
	Lvector  h(L);
	int i;
	for (i = 0; i < L; i++)
	{
		h[i] = (i==n)*H;
	}

	for (i = 0; i < 5; i++)
	{
		Lvector dp = h * (i-2);
		tensor2 metr = Metric(p + dp);
		res += metr*(coef[i]);
	}


	return res;
}
*/
/*
tensor2 dGi(Lvector p, int n)
{
	int L = p.dim();
	tensor2 res(L);
	real  coef[3] = {-1/(2*H), 0, 1/(2*H)};
	Lvector  h(L);
	int i;
	for (i = 0; i < L; i++)
	{
		h[i] = (i==n)*H;
	}

	for (i = 0; i < 3; i++)
	{
		Lvector dp = h * (i-1);
		tensor2 metr = Metric(p + dp);
		res += metr*(coef[i]);
	}


	return res;
}

*/

tensor3 Kristofel(Lvector p)
{
	int L = p.dim();
	tensor3 res(L), kr(L);
	int i,j,k,l;
	tensor3 dg(L);

	for (i = 0; i < L; i++)
	{
		dg[i] = dGi(p, i);
	}

	for (i = 0; i < L; i++)
	for (j = 0; j < L; j++)
	for (k = 0; k < L; k++)
	{
		kr[i][j][k] = 0.5 * (dg[j][i][k] + dg[k][i][j] - dg[i][j][k]);
	}
	tensor2 G = Metric(p);
	G = G.inv();

	for (j = 0; j < L; j++)
	for (k = 0; k < L; k++)
	for (i = 0; i < L; i++)
		res[i][j][k] = 0;
	for (j = 0; j < L; j++)
	for (k = 0; k < L; k++)
	for (i = 0; i < L; i++)
	for (l = 0; l < L; l++)
	{
		res[i][j][k] += kr[l][j][k] * G[i][l];
	}


	return res;
}


/*
tensor3 Kristofel(Lvector p)
{
	
	tensor3 res(2), kr(2);
	int i, j, k, l;
	
  	for (j = 0; j < 2; j++)
	for (k = 0; k < 2; k++)
	for (i = 0; i < 2; i++)
	{
		kr[i][j][k] = res[i][j][k] = 0;
	}
	double x1, x2;
	x1 = p[0];
	x2 = p[1];
	
	kr[0][0][0] = kr[0][1][1] = a*x1;
	kr[1][0][0] = kr[1][1][1] = a*x2;
	
	tensor2 G(2);
	
	
	G = Metric(p);
	G = G.inv();
	
	
	
	
	for (i = 0; i < 2; i++)
	for (j = 0; j < 2; j++)
	for (k = 0; k < 2; k++)
	for (l = 0; l < 2; l++)
	{
	  res[i][j][k] += G[i][l]*kr[l][j][k];
	}
	
	
	return res;
}
*/