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

metaclass VerboseClass Person;    // metaclass declaration
class Person {
public:
    Person(int i) { age = i; }
    int Age() { return age; }
    int BirthdayComes() { return ++age; }
private:
    int age;
};

int main()
{
    Person billy(24);
    printf("age %d\n", billy.Age());
    printf("age %d\n", billy.BirthdayComes());

    return 0;
}

