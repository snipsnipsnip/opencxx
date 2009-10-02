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

#include "matrix.h"

int main()
{
    Matrix r = { 0.5, -0.86, 0, 0.86, 0.5, 0, 0, 0, 1 };
    Matrix s = { 0.86, -0.5, 0, 0.5, 0.86, 0, 0, 0, 1 };
    Matrix t;
    double d = 2.2;

    t.forall(e){ e = 0.1; };

    t = d * t + r * s;
    t.Print();

    return 0;
}
