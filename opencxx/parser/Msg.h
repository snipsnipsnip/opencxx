#ifndef guard_opencxx_Msg_h
#define guard_opencxx_Msg_h

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

#include <iosfwd>

namespace Opencxx
{

class SourceLocation;

class Msg
{
public:
    enum Severity { Info, Warning, Error, Fatal };
    virtual Severity GetSeverity() const = 0;
    virtual void PrintOn(std::ostream&) const = 0;
};

}

#endif /* ! guard_opencxx_Msg_h */
