/** File eye.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */



#include "geod.h"
#include "eye.h"
#include "task.h"

bool eye::if_in_eye(poskas pk)
{
  int i;
  int dim = pk.p.dim();
  if (fabs(pk.p[0]-pos[0]) > dt)
    return false;
  for (i = 1; i < dim; i++)
  {
    if (fabs(pk.p[i]-pos[i]) > size)
      return false;  
  }
  
  return true;
}

real eye::angle_dir(poskas pk)
{
  real cs = 0, ca = 0, cb = 0;
  int i, j;
  tensor2 G = Metric(pk.p);
  int dim = pk.p.dim();
  tensor2 G3 = spacesubm(G);
  Lvector dir3 = spacesubv(dir);
  Lvector vel = spacesubv(pk.v);
  
  
  
  ca = G3.scalar(vel, vel);
  
  if (ca < 0)
    throw 3;
  ca = sqrt(ca);
  vel /= ca;
  
  cs = G3.scalar(dir3, vel);
  
  return acos(-cs);
}


real eye::angle_top(poskas pk)
{
  real ca, cs, cw;
  tensor2 G3 = spacesubm(Metric(pk.p));
  Lvector dir3 = spacesubv(dir);
  Lvector top3 = spacesubv(top);
  Lvector vel = spacesubv(pk.v);
  
  
  ca = G3.scalar(vel, vel);
  
  ca = sqrt(ca);
  
  vel /= ca;
  
  
  cs = G3.scalar(dir3, vel);
  vel -= (dir3*cs);
  
  cw = G3.scalar(top3, vel);
  
  return acos(-cw);
}
