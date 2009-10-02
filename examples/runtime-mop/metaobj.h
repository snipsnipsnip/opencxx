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

#ifndef __metaobj_h
#define __metaobj_h

// Runtime Meta Level

extern char mBuffer[];
extern int mIndex;

/*
  MetaObj is the root class for runtime metaobjects.  The runtime
  metaobject traps member function calls on the base-level object.

  If it traps a member function call, Reflect() is invoked with the
  identifier of the member function (member), the byte array containing
  the argument list (arglist), and the length of that byte array
  (arglist_size).

  Reify() is used to actually execute a member function on the base-
  level object.
*/

class MetaObj {
public:
    virtual void Reflect(int member, char* arglist, int arglist_size);
    virtual void Reify(int member, char* arglist) = 0;
};

#endif  /* __metaobj_h */
