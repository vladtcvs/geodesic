/** File space.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#ifndef SPACE_H
#define SPACE_H

#include "runge.h"


tensor2 dGi(Lvector p, int n);
tensor3 Kristofel(Lvector p);
poskas space_diff(poskas pk);


class diff_fun_space : public diff_fun
{
	public:
		poskas diff(poskas pk);
};

#endif

