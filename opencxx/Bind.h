#ifndef guard_opencxx_Bind_h
#define guard_opencxx_Bind_h

//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{contributors}{}@
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
//  Other Contributors (see file AUTHORS) make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  Other Contributors (see file AUTHORS)
//
//@endlicenses@

#include <opencxx/parser/GC.h>
#include <opencxx/defs.h>

namespace Opencxx
{

class Class;
class TypeInfo;
class Environment;

class Bind : public LightObject {
public:
    enum Kind {
	isVarName, isTypedefName, isClassName, isEnumName, isTemplateClass,
	isTemplateFunction
     };
    virtual Kind What() = 0;
    virtual void GetType(TypeInfo&, Environment*) = 0;
    virtual char* GetEncodedType();
    virtual bool IsType();
    virtual Class* ClassMetaobject();
    virtual void SetClassMetaobject(Class*);
};

}

#endif /* ! guard_opencxx_Bind_h */
