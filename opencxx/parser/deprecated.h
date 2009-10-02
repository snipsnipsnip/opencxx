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

#ifndef guard_opencxx_deprecated_h
#define guard_opencxx_deprecated_h

namespace Opencxx
{

void Deprecated(const char* functionName, const char* replacement = 0);

#undef OPENCXX_DEPRECATE_NOW

#ifdef OPENCXX_DEPRECATE_NOW
#   define OPENCXX_DEPRECATED_PUBLIC    private
#   define OPENCXX_DEPRECATED_PROTECTED private
#else
#   define OPENCXX_DEPRECATED_PUBLIC    public
#   define OPENCXX_DEPRECATED_PROTECTED protected
#endif

}

#endif /* ! guard_opencxx_deprecated_h */
