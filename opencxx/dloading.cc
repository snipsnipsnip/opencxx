//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{xerox}{}@
//
//  Copyright (C) 1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
//
//  Permission to use, copy, distribute and modify this software and
//  its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appears in all copies and that
//  both that copyright notice and this permission notice appear in
//  supporting documentation.
//
//  Shigeru Chiba makes no representations about the suitability of this
//  software for any purpose.  It is provided "as is" without express or
//  implied warranty.
//
//  -----------------------------------------------------------------
//
//
//  Copyright (c) 1995, 1996 Xerox Corporation.
//  All Rights Reserved.
//
//  Use and copying of this software and preparation of derivative works
//  based upon this software are permitted. Any copy of this software or
//  of any derivative work must include the above copyright notice of   
//  Xerox Corporation, this paragraph and the one after it.  Any
//  distribution of this software or derivative works must comply with all
//  applicable United States export control laws.
//
//  This software is made available AS IS, and XEROX CORPORATION DISCLAIMS
//  ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE  
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR    
//  PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
//  LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
//  EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
//  NEGLIGENCE) OR STRICT LIABILITY, EVEN IF XEROX CORPORATION IS ADVISED
//  OF THE POSSIBILITY OF SUCH DAMAGES.
//
//@endlicenses@

#include <opencxx/dloading.h>
#include <iostream>
#include <cstdlib>
#include <opencxx/defs.h>
#include "opencxx/parser/CerrErrorLog.h"
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/GenericMsg.h>

namespace Opencxx
{

void* LookupSymbol(lt_dlhandle handle, const char* symbol)
{
    void* func = 0;
#if USE_DLOADER
    func = lt_dlsym(handle, symbol);
    if (func == 0) {
 	std::cerr << "lt_dlsym() failed (non metaclass?): " << lt_dlerror() << '\n';
 	std::exit(1);
    }
#endif
    return func;
}

lt_dlhandle LoadSoLib(const char* file_name)
{
    lt_dlhandle handle = 0;
#if USE_DLOADER
    //handle = dlopen(file_name, RTLD_GLOBAL | RTLD_LAZY);
    //handle = dlopen(file_name, RTLD_GLOBAL | RTLD_LAZY);
    handle = lt_dlopen(file_name);
    if(handle == NULL){
 	std::cerr << "lt_dlopen(" << file_name << ") failed: " << lt_dlerror() << '\n';
 	exit(1);
    }
#endif /* USE_DLOADER */

    return handle;
}


void
InitDynamicLoader(ErrorLog& errorLog)
{
#if SHARED_OPTION
    if (!lt_dlinit()) {
        lt_dladdsearchdir(".");
    }
    else {
        errorLog.Report(
            GenericMsg(
                Msg::Warning, SourceLocation(),
                "warning: cannot initialize dynamic loader"
            )
        );
    }
#endif
}

void
ExitDynamicLoader()
{
#if SHARED_OPTION
    lt_dlexit();
#endif
}

}

