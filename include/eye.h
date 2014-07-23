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

#include <geod.h>
#include <tensor.h>

struct obspnt
{
  bool u;
  real ang, dir;
};


class eye
{
Lvector pos;
Lvector dp;


Lvector dir3;
Lvector top3;
tensor2 G3;


  
  real angle_dir(poskas pk);
  real angle_top(poskas pk);
public:
  bool if_in_eye(poskas pk);
  eye(){ }
  void init(Lvector npos, Lvector ntop, Lvector ndir, Lvector ndp);
  obspnt observe(poskas pk);
};


#endif
