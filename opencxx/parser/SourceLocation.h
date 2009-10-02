#ifndef guard_opencxx_SourceLocation_h
#define guard_opencxx_SourceLocation_h

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

#include <string>
#include <cassert>

namespace Opencxx
{

class SourceLocation
{
private:
    enum { NONE = -1 };

public:
    SourceLocation()
      : lineNo_(NONE)
    {
    }

    SourceLocation(const std::string& filename, int lineNo)
      : filename_(filename)
      , lineNo_(lineNo)
    {
        assert(lineNo_ >= 0);
    }
    
    std::string GetFilename() const
    { 
        assert(lineNo_ != NONE);
        return filename_; 
    }
    
    int GetLineNo() const
    { 
        assert(lineNo_ != NONE);
        return lineNo_; 
    }
    
    // conversion to "bool"
    typedef int (SourceLocation::*UnspecifiedBoolType)() const;
    operator UnspecifiedBoolType() const
    {
        return lineNo_ == NONE ? 0 : &SourceLocation::GetLineNo;
    }
private:
    std::string filename_;
    int         lineNo_;
};

}

#endif /* ! guard_opencxx_SourceLocation_h */
