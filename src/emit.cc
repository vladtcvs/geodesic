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

#include "errors.h"

/**
 * This function creates new object, moving with speed vel and
 * with direction dir
 */
poskas emit_light(Lvector pos, Lvector dir)
{
  poskas res;
  tensor2 G = Metric(pos);
  int i, j;
  int D = dir.dim(); 
  real a = 0, b = 0, k=G[0][0];
  
  for (i = 1; i < D; i++)
  for (j = 1; j < D; j++)
    b += G[i][j]*dir[i]*dir[j];
  
  for (i = 1; j < D; i++)
    a += G[0][i]*dir[i];
  
  if (fabs(k) < eps)
    throw EMETR;
  b /= k;
  a /= k;
  
  if (b >= 0)
    throw EMETR;
  
  real x0 = -a + sqrt(a*a-b);
  
  res.p = pos;
  res.v = dir;
  res.v[0] = x0;

#ifdef DEBUG
  real s = 0;
  for (i = 0; i < D; i++)
  for (j = 0; j < D; j++)
    s += G[i][j]*res.v[i]*res.v[j];
  if (fabs(s) > eps)
  {
    printf("emit error\n");
    throw EMETR;
  }
#endif

  return res;
}


/**
 * This function creates new object, moving with speed vel and
 * with  direction dir
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
  {
    throw EDIM;
  }
  for (i = 1; i < D; i++)
  {
    if (G[0][i] != 0 || G[i][0] != 0)
    {
      throw ETYPE;
    }
    
  }
  if (G[0][0] < 0)
    throw ETYPE;
  
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
