#ifndef CLIENT_H
#define CLIENT_H

#include "geod.h"

#ifdef LINUX

void* geodesic_pthread(void* data);
#elif WINDOWS
DWORD WINAPI geodesic_winthreads( LPVOID data );
#endif


#endif
