#ifndef guard_opencxx_MopMsg_h
#define guard_opencxx_MopMsg_h

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
#include <string>
#include <cassert>
#include <opencxx/parser/Msg.h>

namespace Opencxx
{

class MopErrorException {};

class MopMsg : public Msg
{
public:
    MopMsg(
        Severity severity, 
        const std::string& 
        where, const std::string& what
    )
      : severity_(severity)
      , where_(where)
      , what_(what)
    {
    }
    
    MopMsg(Severity severity, const std::string& what)
        : severity_(severity)
        , what_(what)
    {
    }
    
    virtual ~MopMsg() {}

    virtual void PrintOn(std::ostream& os) const
    {
        os << "occ: MOP " << Category() << ": ";
        if (where_ != std::string()) 
        {
            os << "in " << where_ << ", ";
        }
        os << what_ << std::endl;
    }
    
    virtual Severity GetSeverity() const 
    { 
        return severity_; 
    }
private:

    std::string Category() const
    {
        switch (severity_)
        {
            case Error:   return "error";
            case Fatal:   return "error";
            case Warning: return "warning";
            default:      assert(! "unknown category");
        }
        abort();
    }

private:
    const Severity    severity_;
    const std::string where_;
    const std::string what_;
};

}

#endif /* ! guard_opencxx_MopMsg_h */
