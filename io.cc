#include <stdio.h>
#include "geod.h"


void ioid::open(int ind)
{
  char fname[10];
  sprintf(fname, "out%05i", ind);
  out = fopen(fname, "wt");
}


void ioid::close()
{
  if (out)
    fclose(out);
  out = NULL;
}

int ioid::write_poskas(poskas pk)
{
  int L = pk.p.dim();
  int i;
  for (i = 0; i < L; i++)
  {
    if (fabs(pk.p[i]) > 1e20 || fabs(pk.v[i]) > 1e20)
      return -1;
  }
  
  
  for (i = 0; i < L; i++)
  {
    fprintf(out, "%lf ", (double)(pk.p[i]));
  }
  for (i = 0; i < L; i++)
  {
    fprintf(out, "%lf ", (double)(pk.v[i]));
  }
  fprintf(out, "\n");
  return 0;
}
