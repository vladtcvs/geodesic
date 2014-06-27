/** File runge.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#include "runge.h"


void solver::init(diff_fun* df, poskas pos, real dh)
{
	fun = df;
	start = pos;
	dl = dh;
}

poskas solver::makestep(real len)
{
	poskas pk = integrate(len);
	start = pk;
	return pk;
}




poskas solver_rk4::integrate(real len)
{
    poskas pk=start;
    real h = 0;
    do
    {
      poskas pp = runge_kutta4(pk,dl);
      if (pp.p.dim() != pk.p.dim())
	throw EDIM;
      pk = pp;
      h += dl;
    } 
    while(h <= len);
  
    return pk;
}


poskas solver_rk4::runge_kutta4(poskas &pk, real h)
{
	poskas res, k1, k2, k3, k4;
	//PRINT_LOG
	
	k1 = fun->diff(pk)*h;
	k2 = fun->diff(pk + k1/2)*h;
	k3 = fun->diff(pk + k2/2)*h;
	k4 = fun->diff(pk + k3)*h;
	return pk + (k1+k2*2+k3*2+k4)/6;
}


