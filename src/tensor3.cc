/** File tensor3.cc author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */

#include <geod.h>
#include <tensor.h>

tensor3::tensor3()
{
	L = deflen;
	m.resize(L);
}

void tensor3::alloc(int n)
{
	L = n;
	m.resize(L);
	for (int j = 0; j < L; j++)
	{
		m[j].alloc(L);
	}
}

tensor3::tensor3(int i)
{
	L = i;
	int j;
	m.resize(L);
	for (j = 0; j < L; j++)
	{
		m[j].alloc(L);
	}
}

tensor3::~tensor3()
{

}

tensor2& tensor3::operator [] (int i)
{
	if (i < 0 || i >= L)
	{
	  PRINT_LOG
		throw EIND;
	}
	return m[i];
}

