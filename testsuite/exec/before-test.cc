//(Automatic CVS control) $Revision: 1.6 $ $Date: 2004/06/09 14:56:15 $

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

metaclass BeforeClass Queue;

const int SIZE = 8;

class Queue {
public:
    Queue(){ i = 0; }
    void Put(int);
    void before_Put();
    int Peek();

private:
    int buffer[SIZE];
    int i;
};

void Queue::Put(int x)
{
    puts("Put() is invoked.");
    buffer[i++ % SIZE] = x;
}

// before_Put() should be automatically invoked before Put() is invoked.

void Queue::before_Put()
{
    puts("before_Put() is invoked.");
    puts("synchronize... suspend the thread while the buffer is full.");
}

int Queue::Peek()
{
    puts("Peek() is invoked.");
    return buffer[(i - 1) % SIZE];
}


int main()
{
    Queue q;
    puts("call q.Put(3)");
    q.Put(3);
    puts("call q.Peek(3)");
    int k = q.Peek();
}
