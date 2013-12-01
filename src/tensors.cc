/** File tensors.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#include "geod.h"



int m1p(int i, int j)
{
  return (1-2*((i+j)%2));
}

void Lvector::alloc(int i)
{
	L = i;
	V.resize(L);
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
		throw 2;
	}
	return V[i];
}

Lvector Lvector::operator + (Lvector &v)
{
	int i;
	if (L != v.L)
	{
	  PRINT_LOG
		throw 1;
	}
	Lvector res(L);
	for (i = 0; i < L; i++)
		res[i] = v.V[i] + V[i];
	return res;
}


Lvector Lvector::operator - (Lvector &v)
{
	int i;
	if (L != v.L)
		throw 1;
	Lvector res(L);
	for (i = 0; i < L; i++)
		res[i] = V[i] - v.V[i];
	return res;
}


Lvector Lvector::operator += (Lvector &v)
{
	int i;
	if (L != v.L)
	{
	  PRINT_LOG
		throw 1;
	}
	for (i = 0; i < L; i++)
		 V[i] += v.V[i];
	return *this;
}

Lvector Lvector::operator -= (Lvector &v)
{
	int i;
	if (L != v.L)
	{
	  PRINT_LOG
		throw 1;
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
		throw 0;
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
		throw 0;
	}
	for (i = 0; i < L; i++)
		V[i] /= n;
	return (*this);
}



tensor2 tensor2::operator / (real n)
{
	tensor2 res(L);
	if (fabs(n) < eps)
	{	
		PRINT_LOG
		throw 0;
	}
	int i;
	for (i = 0; i < L; i++)
		res[i] = m[i]/n;
	return res;
}


tensor2 tensor2::operator += (tensor2 t)
{
	if(L!=t.dim())
	{
	  PRINT_LOG
		throw 1;
	}
	int i;
	for (i = 0; i < L; i++)
		m[i] += t.m[i];
	return *this;
}

tensor2 tensor2::operator * (real n)
{
	tensor2 res(L);
	int i;
	for (i = 0; i < L; i++)
		res[i] = m[i]*n;
	return res;
}



tensor2 tensor2::operator *= (real n)
{
	int i;
	for (i = 0; i < L; i++)
		m[i]*=n;
	return *this;
}



tensor2 tensor2::adj(int i, int j)
{
	int a, b, sa = 0, sb = 0;

	tensor2 sub(L-1);
	for (a = 0; a < L; a++)
	{	
		if (a == i)
			continue;
		sb = 0;
		
		for (b = 0; b < L; b++)
		{
			if (b == j)
				continue;
			sub[sa][sb] = m[a][b];
			sb++;
		}
		
		sa++;
	}
	return sub;
}

real tensor2::det()
{
	int i;
	real s = 0;
	int c = 1;

	if (L == 0)
	{
	  PRINT_LOG
		throw 2;
	}
	if (L == 1)
	{
	  
		return m[0][0];
	}
	if (L == 2)
		return m[0][0]*m[1][1] - m[0][1]*m[1][0];

	for (i = 0; i < L; i++)
	{
		tensor2 sub = adj(0, i);
		s += c*sub.det()*m[0][i];
		c = -c;
	}

	return s;
}



tensor2 tensor2::inv()
{
	real  d = det();
	if (fabs(d)< eps)
	{
	  PRINT_LOG
		throw 0;
	}
	tensor2 res(L);
	int i, j;
	for (i = 0; i < L; i++)
	for (j = 0; j < L; j++)
	{
		tensor2 A = adj(j,i);
		res[i][j] = m1p(i,j)* A.det()/d;
	}
	return res;
}

tensor2::tensor2()
{
	L = deflen;
	m.resize(L);
}

void tensor2::alloc(int n)
{
	L = n;
	m.resize(L);
	for (int j = 0; j < L; j++)
	{
		m[j].alloc(L);
	}
}

tensor2::tensor2(int i)
{
	L = i;
	int j;
	m.resize(L);
	for (j = 0; j < L; j++)
	{
		m[j].alloc(L);
	}
}

tensor2::~tensor2()
{

}

Lvector& tensor2::operator [] (int i)
{
	if (i < 0 || i >= L)
	{
	  PRINT_LOG
		throw 2;
	}
	return m[i];
}


double tensor2::scalar(Lvector a, Lvector b)
{
  double s = 0;
  int i, j;
  int D = dim();
  
  if (a.dim() != D || b.dim() != D)
    return -BAD_DOUBLE;;
  for (i = 0; i < D; i++)
  for (j = 0; j < D; j++)
    s += m[i][j] * a[i] * b[j];
  return s;
}



tensor3::tensor3()
{
	L = deflen;
	m.resize(L);
}

void tensor3::alloc(int n)
{
	L = n;
	m.resize(L);
	for (int j = 0; j < L; j++)
	{
		m[j].alloc(L);
	}
}

tensor3::tensor3(int i)
{
	L = i;
	int j;
	m.resize(L);
	for (j = 0; j < L; j++)
	{
		m[j].alloc(L);
	}
}

tensor3::~tensor3()
{

}

tensor2& tensor3::operator [] (int i)
{
	if (i < 0 || i >= L)
	{
	  PRINT_LOG
		throw 2;
	}
	return m[i];
}



poskas poskas::operator + (poskas pos)
{
	poskas res;
	res.p = p + pos.p;
	res.v = v + pos.v;
	return res;
}

poskas poskas::operator - (poskas pos)
{
	poskas res;
	res.p = p - pos.p;
	res.v = v - pos.v;
	return res;
}

poskas poskas::operator += (poskas pos)
{
	p += pos.p;
	v += pos.v;
	return *this;
}

poskas poskas::operator -= (poskas pos)
{
	p -= pos.p;
	v -= pos.v;
	return *this;
}

poskas poskas::operator * (real n)
{
	poskas res;
	res.p = p*n;
	res.v = v*n;
	return res;
}

poskas poskas::operator / (real n)
{
	poskas res;
	res.p = p/n;
	res.v = v/n;
	return res;
}

poskas poskas::operator *= (real n)
{
	p*=n;
	v*=n;
	return *this;
}

poskas poskas::operator /= (real n)
{
	p*=n;
	v*=n;
	return *this;
}


poskas::poskas()
{
	p.alloc(deflen);
	v.alloc(deflen);
}

poskas::poskas(int l)
{
	p.alloc(l);
	v.alloc(l);
}



int posopr(tensor2 t)
{
  int pos = 0;
  int k = 1;
  double Det = t.det();
  if (Det > eps)
    pos = 1;
  else if (Det < -eps)
    pos = -1;
  else
    return pos;
  
  int D = t.dim();
  if (D == 1)
    return pos;
  
  int i, j;
  tensor2 subt;
  
  
  if (pos == -1)
  {
    t *= -1;
    pos = 1;
    k = -1;
  }
  for (i = 1; i <= D-1; i++)
  {
    subt.alloc(i);
   
    for (j = 0; j <= D-i; j++)
    {
      int x, y;
      for (x = 0; x < i; x++)
      for (y = 0; y < i; y++)
      {
	subt[x][y] = t[x+j][y+j];
	
      }
      double DD = subt.det();
      if (DD > eps && pos == -1 || DD < -eps && pos == 1 || fabs(DD) < eps)
	return 0;
    }
  }
  return k;
}




