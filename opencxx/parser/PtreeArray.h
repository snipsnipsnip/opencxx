#ifndef guard_opencxx_PtreeArray_h
#define guard_opencxx_PtreeArray_h

//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{Grzegorz Jakacki}{2004}@
//
//  Copyright (C) 1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
//
//  Permission to use, copy, distribute and modify this software and
//  its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appears in all copies and that
//  both that copyright notice and this permission notice appear in
//  supporting documentation.
//
//  Shigeru Chiba makes no representations about the suitability of this
//  software for any purpose.  It is provided "as is" without express or
//  implied warranty.
//
//  -----------------------------------------------------------------
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  Grzegorz Jakacki make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C) 2004 Grzegorz Jakacki
//
//@endlicenses@

#include <opencxx/parser/GC.h>
#include <opencxx/parser/Ptree.h>

namespace Opencxx
{

class PtreeArray : public LightObject {
public:
    PtreeArray(int = 8);
    unsigned Number() { return num; }
    Ptree*& operator [] (unsigned index) { return Ref(index); }
    Ptree*& Ref(unsigned index);
    void Append(Ptree*);
    void Clear() { num = 0; }
    Ptree* All();

private:
    unsigned num, size;
    Ptree** array;
    Ptree* default_buf[8];
};

}

#endif /* ! guard_opencxx_PtreeArray_h */
