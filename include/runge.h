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

typedef poskas (*runge_fun)(poskas pk);


poskas runge_run(poskas pos, real dh, real hmax);
void set_runge_fun(runge_fun sfun);

#endif
