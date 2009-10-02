#ifndef guard_opencxx_ParseErrorMsg_h
#define guard_opencxx_ParseErrorMsg_h

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
#include <opencxx/parser/SourceLocation.h>

namespace Opencxx
{

class ParseErrorMsg : public Msg
{
public:
    ParseErrorMsg(const SourceLocation& location, const std::string& text)
      : location_(location)
      , text_(text)
    {
    }

    virtual ~ParseErrorMsg() {}

    void PrintOn(std::ostream& os) const
    {
        os << location_.GetFilename() << ":" << location_.GetLineNo()
           << ": parse error before `" << text_ << "'";
    }
    
    Severity GetSeverity() const
    {
        return Error;
    }
private:
    SourceLocation location_;
    std::string    text_;
};

}

#endif /* ! guard_opencxx_ParseErrorMsg_h */
