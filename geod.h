#ifndef GEOD_H
#define GEOD_H
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>

#define WINDOWS 1

#define LINUX 1


#ifdef LINUX
#undef WINDOWS
#endif


#ifdef WINDOWS
#include <Windows.h>
#elif LINUX
#include <glib.h>
#endif

#include "tensor.h"



extern const int deflen;
extern const real eps;
extern const real H;








tensor2 dGi(Lvector p, int n);
tensor3 Kristofel(Lvector p);
tensor2 Metric(Lvector p);
poskas runge_kutta4(poskas &pk, real h);



extern char server_ip[20];
extern char my_ip[20];

#ifdef LINUX
gpointer geodesic_glib(gpointer data);
#elif WINDOWS
DWORD WINAPI geodesic_winthreads( LPVOID data );
#endif

#ifdef DEBUG
#define PRINT_LOG printf("%s (%s:%i)\n", __FUNCTION__, __FILE__, __LINE__);
#else
#define PRINT_LOG {;}
#endif


#endif
