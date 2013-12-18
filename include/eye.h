/*
 *  File eye.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __EYE_H__
#define __EYE_H__

#include "geod.h"

class eye
{
Lvector pos;
Lvector dir;
Lvector top;
real size;
real dt;
real aov;
public:
  bool if_in_eye(poskas pk);
  real angle_dir(poskas pk);
  real angle_top(poskas pk);
};



#endif