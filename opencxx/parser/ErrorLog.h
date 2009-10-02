#ifndef guard_opencxx_ErrorLog_h
#define guard_opencxx_ErrorLog_h

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

namespace Opencxx
{

class Ptree;
class Msg;

class TooManyErrorsException {};
class FatalErrorException {};

class ErrorLog
{
public:
    virtual void Report(const Msg&) = 0;
        /* throws TooManyErrorsException, FatalErrorException */
        
    virtual ~ErrorLog() {}
};

}

#endif /* ! guard_opencxx_ErrorLog_h */
