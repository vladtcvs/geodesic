/** File macros.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#ifndef MACROS_H
#define MACROS_H


#define WINDOWS 0
#define MINIX  0
#define LINUX 1

#include <stdio.h>

#ifdef DEBUG
#define PRINT_LOG printf("%s (%s:%i)\n", __FUNCTION__, __FILE__, __LINE__);
#else
#define PRINT_LOG {;}
#endif


#endif