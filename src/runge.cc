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




runge_fun fun;

void set_runge_fun(runge_fun sfun)
{
  fun = sfun;
}


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

poskas runge_run(poskas pos, real dh, real hmax)
{
    poskas pk=pos;
    real h = 0;
    do
    {
      pk = runge_kutta4(pk,dh);
      h += dh;
    } 
    while(h <= hmax);
  
    return pk;
}
