/** File errors.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */



#include "errors.h"

std::string error_str(int errnum)
{
  std::string ans;
  switch(errnum)
  {
    case ENO:
      ans = "No error";
      break;
    case EZERO:
      ans = "Division by zero";
      break;
    case EDIM:
      ans = "Dimension mismatch";
      break;
    case EMEM:
      ans = "Memory allocation fail";
      break;
    case ETYPE:
      ans = "Wrong object type/form";
      break;
    case EIND:
      ans = "Index out of range";
      break;
    default:
      ans = "Error number " + errnum;
      break;
  }
  
  return ans;
}