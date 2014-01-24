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
  
  for (i = 0; i < dim; i++)
  {
    if (fabs(pk.p[i]-pos[i]) > dp[i])
      return false;  
  }
  
  return true;
}

real eye::angle_dir(poskas pk)
{
  real cs = 0, ca = 0, cb = 0;
  int i, j;
  
  
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
  
  Lvector vel = spacesubv(pk.v);
  
  
  ca = G3.scalar(vel, vel);
  
  ca = sqrt(ca);
  
  vel /= ca;
  
  
  cs = G3.scalar(dir3, vel);
  vel -= (dir3*cs);
  
  cw = G3.scalar(top3, vel);
  
  return acos(-cw);
}

obspnt eye::observe(poskas pk)
{
  obspnt res;
  res.u = if_in_eye(pk);
  if (res.u)
  {
    res.ang = angle_dir(pk);
    res.dir = angle_top(pk);
  }
  
  return res;
}


void eye::init(Lvector npos, Lvector ntop, Lvector ndir, Lvector ndp)
{
  PRINT_LOG;
  dp = ndp;
  
  pos = npos;
  
  tensor2 G = Metric(npos);
  G3 = spacesubm(G);
  
  real ca = G3.scalar(ndir, ndir);
  ca = sqrt(ca);
  ndir /= ca;
  
  real cw = G3.scalar(ntop, ndir);
  ntop -= ndir*cw;
  
  real cb = G3.scalar(ntop, ntop);
  cb = sqrt(cb);
  ntop /= cb;
  
  dir3 = spacesubv(ndir);

  top3 = spacesubv(ntop);
}
