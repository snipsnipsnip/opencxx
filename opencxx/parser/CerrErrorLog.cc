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

#include <opencxx/parser/Msg.h>
#include <opencxx/parser/CerrErrorLog.h>

namespace Opencxx
{

using std::cerr;
using std::endl;

void CerrErrorLog::Report(const Msg& msg) /* throws FatalErrorException */
{
    msg.PrintOn(cerr);
    cerr << endl;
    switch (msg.GetSeverity())
    {
        case Msg::Error: 
            ++errorCount_; 
            if (errorCount_ >= 10)
            {
                throw TooManyErrorsException();
            }
        break;
        
        case Msg::Fatal: 
            throw FatalErrorException(); 
            break;
	default:
	    ;
    }
}

}

