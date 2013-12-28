/** File tensor1.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#include "tensor.h"


void Lvector::alloc(int i)
{
	L = i;
	int k;
	V.resize(L);
	for (k = 0; k < L; k++)
	  V[k] = 0;
}


Lvector::Lvector()
{
	L = deflen;
	V.resize(L);
}


Lvector::Lvector(int l)
{
	L = l;
	V.resize(L);
}


Lvector::~Lvector()
{
	V.clear();
}


real& Lvector::operator [] (int i)
{
	if (i >= L || i < 0)
	{
	  PRINT_LOG
		throw EIND;
	}
	return V[i];
}

Lvector Lvector::operator + (Lvector v)
{
	int i;
	if (L != v.L)
	{
	  PRINT_LOG
		throw EDIM;
	}
	Lvector res(L);
	for (i = 0; i < L; i++)
		res[i] = v.V[i] + V[i];
	return res;
}


Lvector Lvector::operator - (Lvector v)
{
	int i;
	if (L != v.L)
		throw EDIM;
	Lvector res(L);
	for (i = 0; i < L; i++)
		res[i] = V[i] - v.V[i];
	return res;
}


Lvector Lvector::operator += (Lvector v)
{
	int i;
	if (L != v.L)
	{
	  PRINT_LOG
		throw EDIM;
	}
	for (i = 0; i < L; i++)
		 V[i] += v.V[i];
	return *this;
}

Lvector Lvector::operator -= (Lvector v)
{
	int i;
	if (L != v.L)
	{
	  PRINT_LOG
		throw EDIM;
	}
	for (i = 0; i < L; i++)
		 V[i] -= v.V[i];
	return *this;
}

Lvector Lvector::operator * (real n)
{
	Lvector res(L);
	int i;
	for (i = 0; i < L; i++)
		res[i] = V[i]*n;
	return res;

}

Lvector Lvector::operator / (real n)
{

	if (fabs(n) < eps)
	{
	  PRINT_LOG
		throw EZERO;
	}
	Lvector res(L);
	int i;
	for (i = 0; i < L; i++)
		res[i] = V[i]/n;
	return res;

}



Lvector Lvector::operator *= (real n)
{
	int i;

	for (i = 0; i < L; i++)
		V[i] *= n;
	return (*this);
}

Lvector Lvector::operator /= (real n)
{
	int i;
	if (fabs(n) < eps)
	{
	        PRINT_LOG
		throw EZERO;
	}
	for (i = 0; i < L; i++)
		V[i] /= n;
	return (*this);
}



Lvector spacesubv(Lvector v)
{
  Lvector s;
  int dim = v.dim();
  int i;
  for (i = 0; i < dim; i++)
  {
    s[i] = v[i+1];
  }
  return s;
}
