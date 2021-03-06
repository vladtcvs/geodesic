/*
 *  File emit.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 */

/* inclusion guard */
#ifndef __EMIT_H__
#define __EMIT_H__

#include <geod.h>
#include <tensor.h>

poskas emit_object_vel(Lvector pos, double vel, Lvector dir);
poskas emit_light(Lvector pos, Lvector dir);


#endif /* __EMIT_H__ */
