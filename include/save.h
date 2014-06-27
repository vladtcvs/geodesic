/** File save.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */



#ifndef SAVE_H
#define SAVE_H

#include <stdio.h>
#include "geod.h"
#include "eye.h"

void save_pos(FILE *outf, poskas pk, int calc_id);
void draw_point(FILE *outf, obspnt pnt);

#endif
