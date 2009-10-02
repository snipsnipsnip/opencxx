/*
  Copyright (C) 1997,1998 Shigeru Chiba, University of Tsukuba.

  Permission to use, copy, distribute and modify this software and   
  its documentation for any purpose is hereby granted without fee,        
  provided that the above copyright notice appear in all copies and that 
  both that copyright notice and this permission notice appear in 
  supporting documentation.

  Shigeru Chiba makes no representations about the suitability of this 
  software for any purpose.  It is provided "as is" without express or
  implied warranty.

  July 1997: rewritten by Toru Takimoto for version 2.5.
*/
//@beginlicenses@
//@license{xerox}{}@
//@endlicenses@

#include <stdio.h>
#include "matrix.h"

Matrix& operator + (Matrix& a, Matrix& b)
{
    Matrix* sum = new Matrix;
    for(int i = 0; i < N * N; ++i)
	sum->element[i] = a.element[i] + b.element[i];

    return *sum;
}

Matrix& operator - (Matrix& a, Matrix& b)
{
    Matrix* sum = new Matrix;
    for(int i = 0; i < N * N; ++i)
	sum->element[i] = a.element[i] - b.element[i];

    return *sum;
}

Matrix& operator * (double k, Matrix& a)
{
    Matrix* sum = new Matrix;
    for(int i = 0; i < N * N; ++i)
	sum->element[i] = k * a.element[i];

    return *sum;
}

Matrix& operator * (Matrix& a, Matrix& b)
{
    Matrix* sum = new Matrix;
    for(int i = 0; i < N; ++i)
	for(int j = 0; j < N; ++j){
	    double s = 0.0;
	    for(int k = 0; k < N; ++k)
		s += a.element[i * N + k] * b.element[k * N + j];

	    sum->element[i * N + j] = s;
	}

    return *sum;
}

Matrix::Matrix(double d)
{
    for(int i = 0; i < N * N; ++i)
	element[i] = d;
}

Matrix::Matrix(double* tbl)
{
    for(int i = 0; i < N * N; ++i)
	element[i] = tbl[i];
}

void Matrix::Print()
{
    for(int i = 0; i < N; ++i){
	for(int j = 0; j < N; ++j)
	    printf("%.3f ", Ref(i, j));
	putchar('\n');
    }
}

Matrix& Matrix::operator = (Matrix& a)
{
    for(int i = 0; i < N * N; ++i)
	element[i] = a.element[i];

    return *this;
}
