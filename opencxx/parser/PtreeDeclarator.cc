//@beginlicenses@
//@license{chiba_tokyo}{}@
//@license{chiba_tsukuba}{}@
//@license{chiba_tokyo}{}@
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  1997-2001 Shigeru Chiba, Tokyo Institute of Technology. make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
//
//  -----------------------------------------------------------------
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  1997-2001 Shigeru Chiba, University of Tsukuba. make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  1997-2001 Shigeru Chiba, University of Tsukuba.
//
//  -----------------------------------------------------------------
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  1997-2001 Shigeru Chiba, Tokyo Institute of Technology. make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
//
//@endlicenses@

#include <opencxx/parser/PtreeDeclarator.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/parser/deprecated.h>

namespace Opencxx
{

PtreeDeclarator::PtreeDeclarator(Ptree* list, Encoding& typeEnc, 
                                 Encoding& nameEnc, Ptree* declaredName)
  : NonLeaf(list->Car(), list->Cdr())
{
    Deprecated("PtreeDeclarator::PtreeDeclarator(Ptree*,Encoding&,Encoding&,Ptree*)",
               "PtreeDeclarator(p1,e1.Get(),e2.Get(),p2)");
    type = typeEnc.Get();
    name = nameEnc.Get();
    declared_name = declaredName;
    comments = 0;
}

PtreeDeclarator::PtreeDeclarator(Ptree* list, char* typeEnc, 
                                 char* nameEnc, Ptree* declaredName)
  : NonLeaf(list->Car(), list->Cdr())
  , type(typeEnc)
  , name(nameEnc)
  , declared_name(declaredName)
  , comments(0)
{
}

PtreeDeclarator::PtreeDeclarator(Encoding& encType, Encoding& encName, Ptree* declaredName)
  : NonLeaf(0, 0)
{
    Deprecated("PtreeDeclarator::PtreeDeclarator(Encoding& encType, Encoding& encName, Ptree* declaredName)",
               "PtreeDeclarator(encType.Get(),encName.Get(),declaredName)");
    type = encType.Get();
    name = encName.Get();
    declared_name = declaredName;
    comments = 0;
}

PtreeDeclarator::PtreeDeclarator(char* encType, char* encName, Ptree* declaredName)
  : NonLeaf(0, 0)
  , type(encType)
  , name(encName)
  , declared_name(declaredName)
  , comments(0)
{
}

PtreeDeclarator::PtreeDeclarator(Ptree* p, Ptree* q,
				 Encoding& encType, Encoding& encName, Ptree* declaredName)
  : NonLeaf(p, q)
{
    Deprecated("PtreeDeclarator::PtreeDeclarator(Ptree* p, Ptree* q, Encoding& encType, Encoding& encName, Ptree* declaredName)",
               "PtreeDeclarator(p,q,encType.Get(),encName.Get(),declaredName)");
    type = encType.Get();
    name = encName.Get();
    declared_name = declaredName;
    comments = 0;
}

PtreeDeclarator::PtreeDeclarator(Ptree* p, Ptree* q,
				 char* encType, char* encName, Ptree* declaredName)
  : NonLeaf(p, q)
  , type(encType)
  , name(encName)
  , declared_name(declaredName)
  , comments(0)
{
}

PtreeDeclarator::PtreeDeclarator(Ptree* list, Encoding& encType)
  : NonLeaf(list->Car(), list->Cdr())
{
    Deprecated("PtreeDeclarator::PtreeDeclarator(Ptree* list, Encoding& encType)",
               "PtreeDeclarator(list,encType.Get())");
    type = encType.Get();
    name = 0;
    declared_name = 0;
    comments = 0;
}

PtreeDeclarator::PtreeDeclarator(Ptree* list, char* encType)
  : NonLeaf(list->Car(), list->Cdr())
  , type(encType)
  , name(0)
  , declared_name(0)
  , comments(0)
{
}

PtreeDeclarator::PtreeDeclarator(Encoding& encType)
  : NonLeaf(0, 0)
{
    Deprecated("PtreeDeclarator::PtreeDeclarator(Encoding& encType)",
               "PtreeDeclarator(encType.Get())");
    type = encType.Get();
    name = 0;
    declared_name = 0;
    comments = 0;
}

PtreeDeclarator::PtreeDeclarator(char* encType)
  : NonLeaf(0, 0)
  , type(encType)
  , name(0)
  , declared_name(0)
  , comments(0)
{
}

PtreeDeclarator::PtreeDeclarator(PtreeDeclarator* decl, Ptree* p, Ptree* q)
  : NonLeaf(p, q)
  , type(decl->type)
  , name(decl->name)
  , declared_name(decl->declared_name)
  , comments(0)
{
}

int PtreeDeclarator::What()
{
    return ntDeclarator;
}

char* PtreeDeclarator::GetEncodedType()
{
    return type;
}

char* PtreeDeclarator::GetEncodedName()
{
    return name;
}

void PtreeDeclarator::Print(std::ostream& s, int i, int d)
{
    NonLeaf::Print(s, i, d);
}

}
