#ifndef CLIENT_H
#define CLIENT_H

#include "geod.h"

#if LINUX
void* geodesic_pthread(void* data);
#elif MINIX
void* geodesic_fork(void* data);
#elif WINDOWS
DWORD WINAPI geodesic_winthreads( LPVOID data );
#endif


#endif
