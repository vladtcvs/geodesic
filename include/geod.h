/** File geod.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#ifndef GEOD_H
#define GEOD_H
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>



#if WINDOWS
#include <Windows.h>
#include <stdint.h>
#endif

#include "tensor.h"




extern const real H;








tensor2 dGi(Lvector p, int n);
tensor3 Kristofel(Lvector p);

poskas my_runge_fun(poskas pk);



extern char server_ip[20];
extern char my_ip[20];


#define DIM 4


#endif
