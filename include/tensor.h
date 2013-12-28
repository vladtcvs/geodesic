/** File tensor.h author Vladislav Tcendrovskii
 *  Copyright (c) 2013
 *  This source subjected to the Gnu General Public License v3 or later (see LICENSE)
 *  All other rights reserved
 *  THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 * */


#ifndef TENSOR_H
#define TENSOR_H

#include <vector>
#include <math.h>
#include "macros.h"
#include "errors.h"

typedef long double real;


#define BAD_DOUBLE 1.2345678987654321
class Lvector
{
	private:
		int L;
		std::vector<real> V;
	public:
		Lvector();
		Lvector(int l);
		~Lvector();
		void alloc(int n);
		int dim() {return L;}
		real& operator [] (int i);
		Lvector operator + (Lvector v);
		Lvector operator - (Lvector v);
		Lvector operator += (Lvector v);
		Lvector operator -= (Lvector v);
		Lvector operator * (real n);
		Lvector operator *= (real n);

		Lvector operator / (real n);
		Lvector operator /= (real n);
};

class tensor2
{
	private:
	int L;
	std::vector<Lvector> m;
	public:
	tensor2();
	int dim() {return L;}
	tensor2 adj(int i, int j);
	tensor2 inv();
	real det();
	void alloc(int n);
	tensor2(int i);
	~tensor2();
	Lvector& operator [] (int i);
	tensor2 operator / (real n);
	tensor2 operator * (real n);
	tensor2 operator *= (real n);
	
	tensor2 operator += (tensor2 t);
	
	double scalar(Lvector a, Lvector b);

};

class tensor3
{
	private:
	int L;
	std::vector<tensor2> m;
	public:
	int dim() {return L;}
	tensor3();
	void alloc(int n);
	tensor3(int i);
	~tensor3();
	tensor2& operator [] (int i);

};

class poskas
{
	public:
	Lvector p, v;
	poskas();
	poskas(int l);
	
	poskas operator + (poskas pos);
	poskas operator - (poskas pos);
	poskas operator += (poskas pos);
	poskas operator -= (poskas pos);
	poskas operator * (real n);
	poskas operator / (real n);
	poskas operator *= (real n);
	poskas operator /= (real n);

};

int posopr(tensor2 t);
tensor2 spacesubm(tensor2 G);
Lvector spacesubv(Lvector v);


extern const int deflen;
extern const real eps;


#endif

