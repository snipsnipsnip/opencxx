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

#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/PtreeDeclarator.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/parser/token-names.h>

namespace Opencxx
{

namespace PtreeTypeUtil {

/* const */ char* TheArgumentName()
{
    return "_arg_%d_";
}

Ptree* 
FillArgumentName(Ptree* arg, Ptree* d, int arg_name)
{
    PtreeDeclarator* decl = (PtreeDeclarator*)d;
    if(decl->Name() != 0)
	return arg;
    else{
	unsigned char* type = (unsigned char*)decl->GetEncodedType();
	return Encoding::MakePtree(type,
				   Ptree::Make(TheArgumentName(), arg_name));
    }
}

Ptree* StripCvFromIntegralType(Ptree* integral)
{
    using PtreeUtil::Second;
    if(integral == 0)
	return 0;

    if(!integral->IsLeaf())
	if(integral->Car()->IsA(CONST) ||
	   integral->Car()->IsA(VOLATILE))
	{
	    return Second(integral);
	}
	else {
	    if (Second(integral)->IsA(CONST) || 
	        Second(integral)->IsA(VOLATILE))
            {
                return integral->Car();
            }
        }

    return integral;
}



Ptree* 
GetClassOrEnumSpec(Ptree* typespec)
{
    Ptree* spec = StripCvFromIntegralType(typespec);
    if(spec->IsA(ntClassSpec) || spec->IsA(ntEnumSpec))
	return spec;

    return 0;
}

Ptree* 
GetClassTemplateSpec(Ptree* body)
{
    if(PtreeUtil::Eq(PtreeUtil::Third(body), ';')){
	Ptree* spec = StripCvFromIntegralType(PtreeUtil::Second(body));
	if(spec->IsA(ntClassSpec))
	    return spec;
    }

    return 0;
}



}

}

