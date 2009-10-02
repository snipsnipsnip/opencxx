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

#ifndef __matrix_h
#define __matrix_h

const int N = 3;

#ifdef __opencxx
metaclass MatrixClass Matrix;
#endif

class Matrix {
public:
    Matrix(double);
    Matrix(double*);
    Matrix(){}
    void Print();
    Matrix& operator = (Matrix&);
    double& Ref(int i, int j) { return element[i * N + j]; }

    double element[N * N];
};

Matrix& operator + (Matrix&, Matrix&);
Matrix& operator - (Matrix&, Matrix&);
Matrix& operator * (Matrix&, Matrix&);
Matrix& operator * (double, Matrix&);

#endif /* __matrix_h */
