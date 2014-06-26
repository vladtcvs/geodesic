/** File runge.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#ifndef RUNGE_H
#define RUNGE_H

#include "tensor.h"

typedef poskas (*diff_fun)(poskas pk);

class solver
{
	protected:
		poskas start;
		real dl;
		diff_fun fun;
	public:
		void init(diff_fun df, poskas pos, real dh);
		virtual poskas integrate(real len) = 0;
		poskas makestep(real len);
};

class solver_rk4 : public solver
{
	poskas runge_kutta4(poskas &pk, real h);
	public:
		poskas integrate(real len);
};

#endif



