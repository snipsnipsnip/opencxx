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
#include <opencxx/Class.h>
#include <opencxx/TemplateClass.h>
#include <opencxx/QuoteClass.h>
#include <opencxx/Metaclass.h>
#include <opencxx/MetaclassRegistry.h>

// :WARNING: This file cannot be linked into the opencxx_mop library.
//     If it is linked into the library, linker discards
//     ...Creator objects, because they are not referenced by
//     the executable. 

BEGIN_OPENCXX_NAMESPACE

template <class C>
static
Class*
Create(Ptree* def, Ptree* marg)
{
    Class* metaobject = new C;
    metaobject->InitializeInstance(def, marg);
    return metaobject;
}

void
opcxx_init_Class()
{
    TheMetaclassRegistry().Add(
        "Class", 
        Create<Class>, 
        Class::Initialize,
	0);
}

void
opcxx_init_TemplateClass()
{
    TheMetaclassRegistry().Add(
        "TemplateClass", 
        Create<TemplateClass>,
	TemplateClass::Initialize, 
	0);
}

void
opcxx_init_QuoteClass()
{
    TheMetaclassRegistry().Add(
        "QuoteClass", 
        Create<QuoteClass>,
        QuoteClass::Initialize, 
        0);
}

void
opcxx_init_Metaclass()
{
    TheMetaclassRegistry().Add(
        "Metaclass", 
        Create<Metaclass>,
	Metaclass::Initialize, 
	0);
}

struct Dummy
{
    Dummy()
    {
        opcxx_init_Metaclass();
        opcxx_init_Class();
        opcxx_init_QuoteClass();
        opcxx_init_TemplateClass();
    }
};

namespace {
    Dummy runMyCtorAtStaticInitializationTime;
}

END_OPENCXX_NAMESPACE
