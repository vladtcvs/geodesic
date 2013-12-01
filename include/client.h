/** File client.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


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
