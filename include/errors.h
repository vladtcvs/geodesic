/** File errors.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */



#ifndef ERRORS_H
#define ERRORS_H

#include <string>

#define ENO	0
#define EZERO 	1
#define EDIM	2
#define EMEM	3
#define ETYPE	4
#define EIND	5
#define EMETR	6

std::string error_str(int errnum);

#endif