#ifndef guard_opencxx_MetaclassRegistration_h
#define guard_opencxx_MetaclassRegistration_h

//@beginlicenses@
//@license{contributors}{}@
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  Other Contributors (see file AUTHORS) make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  Other Contributors (see file AUTHORS)
//
//@endlicenses@

#include <opencxx/defs.h>
#include <opencxx/MetaclassRegistry.h>
#include <opencxx/TheMetaclassRegistry.h>
#include <cstdio>

namespace Opencxx
{

class MetaclassRegistration
{
public:
    MetaclassRegistration(const char*   name
      , MetaclassRegistry::Creator*     creator
      , MetaclassRegistry::Initializer* initializer
      , MetaclassRegistry::Finalizer*   finalizer)
    {
        fprintf(stderr, "[MetaclassRegistration(%s)]\n", name);
        TheMetaclassRegistry().Add(name, creator, initializer, finalizer);
    }
};

}

#endif /* ! guard_opencxx_MetaclassRegistration_h */
