#ifndef guard_opencxx_InfoMsg_h
#define guard_opencxx_InfoMsg_h

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

#include <iostream>
#include <opencxx/parser/SourceLocation.h>
#include <opencxx/parser/Msg.h>

namespace Opencxx
{

class InfoMsg : public Msg
{
public:
    InfoMsg(const std::string& text)
      : text_(text)
    {
    }

    virtual ~InfoMsg() {}

    void PrintOn(std::ostream& os) const
    {
        os << "occ: " << text_ << std::endl;
    }
    
    Severity GetSeverity() const
    {
        return Info;
    }
private:
    std::string    text_;
};

}

#endif /* ! guard_opencxx_InfoMsg_h */
