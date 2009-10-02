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

/*
  This is a sample program using RtmopClass.
  We define a new runtime metaobject VerboseObj to control
  the behavior of Person objects.
*/

#include <stdio.h>
#include "metaobj.h"

// --- Runtime Meta Level

class VerboseObj : public MetaObj {
public:
    void Reflect(int member, char* arglist, int arglist_size){
	puts("*** member function call!");
	Reify(member, arglist);
    }
};

// --- Base Level

// Syntax: metaclass RtmopClass <class name>(<runtime metaobject>);

metaclass RtmopClass Person(VerboseObj);

class Person {
public:
    Person(int i) { age = i; }
    int Age() { return age; }

private:
    int age;
};

int main()
{
    Person billy(24);

    printf("age %d\n", billy.Age());
    return 0;
}
