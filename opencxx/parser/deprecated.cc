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

#include <opencxx/parser/deprecated.h>
#include <string>
#include <set>
#include <iostream>

namespace Opencxx
{

void Deprecated(const char* functionName, const char* replacement)
{
   static std::set<std::string> shown;
   std::string name(functionName);
        
   std::set<std::string>::iterator iter = shown.find(name);
   if (iter == shown.end()) {
       shown.insert(iter, name);
       std::cerr << "opencxx-depreciation: " << functionName << " is deprecated";
       if (replacement) {
           std::cerr << "," << std::endl << "opencxx-depreciation: use " 
                     << replacement;
       }
       std::cerr << std::endl;
   }
}

}

