/** File space.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#include "geod.h"
#include "task.h"

#include "space.h"

real a=20;



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
		if (i == 2)
		  continue;
		Lvector dp = h * (i-2);
		tensor2 metr = Metric(p + dp);
		res += metr*((real)(coef[i]));
	}


	return res;
}




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

poskas space_diff(poskas pk)
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

poskas diff_fun_space::diff(poskas pk)
{
	return space_diff(pk);
}

