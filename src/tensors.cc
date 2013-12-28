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

