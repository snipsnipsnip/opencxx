//@beginlicenses@
//@license{Grzegorz Jakacki}{2004}@
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

#include "../opencxx/TheMetaclassRegistry.h"
//#include "../opencxx/MetaclassRegistry.h"

BEGIN_OPENCXX_NAMESPACE

MetaclassRegistry& TheMetaclassRegistry()
{
    static MetaclassRegistry metaclassRegistry;
    return metaclassRegistry;
}

END_OPENCXX_NAMESPACE
