/** File task.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#ifndef TASK_H
#define TASK_H

#include "message.h"
#include "eye.h"
#include <stdio.h>
start_data *srv_get_start();
tensor2 Metric(Lvector p);
void push_pos(poskas pk);
eye get_observer();

void use_result(poskas pk, FILE *outf, int calc_id);
#endif
