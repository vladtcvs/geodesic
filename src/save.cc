/** File save.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */




#include <stdio.h>

#include <geod.h>
#include <save.h>
/**
 * Эта функция запускается только на сервере, она сохраняет принятые от клиентов 
 * данные 
 */
void save_pos(FILE *outf, poskas pk, int calc_id)
{
    int i, L = pk.p.dim();
    PRINT_LOG
    //printf("id = %i L = %i ", calc_id, L);
    fprintf(outf, "%i ", calc_id);
    for (i = 0; i < L; i++)
      fprintf(outf, "%lf ", (double)(pk.p[i]));
    fprintf(outf,"\n");
    fflush(outf);
}

void draw_point(FILE *outf, obspnt pnt)
{
  if (pnt.u)
  {
    int x = pnt.ang * cos(pnt.dir);
    int y = pnt.ang * sin(pnt.dir);
    fprintf(outf, "%lf %lf\n", x, y);
    
  }
}


