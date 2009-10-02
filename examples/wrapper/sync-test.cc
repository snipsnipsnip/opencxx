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

// To see the translated program, see sync-test.ii.

#include <iostream>

using namespace std;

// SyncClass should produce wrapper functions that calls this function.

void synchronize()
{
    cout << "sync!\n";
}

metaclass SyncClass Point;

class Point {
public:
    Point();
    void Move(int x, int);
    int X() { return x; }
    int Y() { return y; }

private:
    int x, y;
};

Point::Point()
{
    x = y = 0;
}

// SyncClass should substitute this member function:
// void Move(int nx, int ny)
// {
//     synchronize();
//     ** call the original Move() **
// }
// for the original Move():

void Point::Move(int nx, int ny)
{
    x = nx;
    y = ny;
}

int main()
{
    Point p;

    p.Move(3, 5);
    int x = p.X();
    int y = p.Y();

    return 0;
}


