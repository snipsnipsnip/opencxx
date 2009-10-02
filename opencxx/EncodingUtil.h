#ifndef guard_opencxx_EncodingUtil_h
#define guard_opencxx_EncodingUtil_h
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

#include <opencxx/PtreeTypeUtil.h>

namespace Opencxx
{

class Environment;

namespace EncodingUtil {

    unsigned char* GetTemplateArguments(unsigned char*, int&);
    char* GetBaseName(char*, int&, Environment*&);

    Environment* ResolveTypedefName(Environment*, char*, int);
    int GetBaseNameIfTemplate(unsigned char*, Environment*&);

}

}

#endif /* ! guard_opencxx_EncodingUtil_h */
