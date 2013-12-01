/*
 * File emit.h author Vladislav Tcendrovskii
 * Copyright (c) 2013
 * This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 * All other rights reserved
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 * OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * 
 */

#include "emit.h"
#include "task.h"

/**
 * This function creates new object, moving with speed vel and
 * with random direction
 */
poskas emit_object_vel(Lvector pos, double vel, Lvector dir)
{
  poskas res;
  tensor2 G = Metric(pos);
  
  /**
   * Тут мы предполагаем, что в метрике нет компонент g_{0,i}
   * Также предполагаем, что метрика имеет сигнатуру (+---)
   * для других ситуаций функция будет работать неверно
   * потому мы проверяем сигнатуру
   * */
  
  int i, j, D = pos.dim(), fail = 0;
  
  if (D != 4)
    return res;
  for (i = 1; i < D; i++)
  {
    if (G[0][i] != 0 || G[i][0] != 0)
    {
      fail=1;
      break;
    }
    
  }
  if (fail == 1)
    return res;
  
  if (G[0][0] < 0)
    return res;
  
  tensor2 Gs;
  Gs.alloc(D-1);
  int x, y;
  for (x = 0; x < D-1; x++)
  for (y = 0; y < D-1; y++)
  {
    Gs[x][y] = G[x+1][y+1];
  }
  
  if (posopr(Gs) != -1)
    return res;
 
  /* сигнатуру проверили */
  
  double u2 = vel*vel;
  Lvector vs;
  vs.alloc(D-1);
  for (i = 0; i < D-1; i++)
    vs[i] = dir[i+1];
  double v2 = Gs.scalar(vs,vs);
  if (v2 == -BAD_DOUBLE || v2 >= -eps)
    return res;
  res.p = pos;
  res.v = dir;
  res.v[0] = sqrt(-v2/(u2*G[0][0]));
  
  return res;
}
