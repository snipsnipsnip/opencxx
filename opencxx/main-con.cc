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

#include <config.h>
#include <iostream>
#include <strstream>
#include <vector>
#include <algorithm>
#include <opencxx/defs.h>
#include <opencxx/driver.h>
#include <opencxx/parser/GC.h>
#include <opencxx/parser/CerrErrorLog.h>
#include <opencxx/OpencxxConfiguration.h>
#include <opencxx/InfoMsg.h>
#include <opencxx/Metaclass.h>
#include <opencxx/UnknownCliOptionException.h>
#include <opencxx/DynamicMopMetaclassLoader.h>
#include <opencxx/TheMetaclassRegistry.h>
#include <opencxx/dloading.h>

using namespace std;
using namespace Opencxx;

static char thisVersion[] = VERSION;

static char copyrightNote[]
        = "Copyright (c) 1997-2001 Shigeru Chiba.  All Rights Reserved.\n"
          "Copyright (c) 1995, 1996 Xerox Corporation. All Rights Reserved.\n"
          "Copyright (c) 2002-2004 Other Developers.\n";



static void ShowHelp(const std::string& argv)
{
    cerr << "Usage: "
         << argv[0]
         << " [-w][-l][-s][-V][-v][-E][-m[<file name>]][-c][-n][-p][--regular-c++]\n"
         << "\t\t[-I<directory>][-D<macro>[=<def>]][-M<option>[=<value>]]\n"
         << "\t\t[-g][-d<option>][-S<metaclass>]\n"
         << "\t\t[-- <compiler options>] <source file>\n"
         << "\n"
         << "    -g             Produce debugging information\n"
         << "    -M             Specify an <option> with <value> passed to metaobjects\n"
         << "    -l             List metaclasses\n"
         << "    -S             Load a metaclass\n"
         << "    -s             Show program tree on stdout\n"
         << "    -V             Show version\n"
         << "    -v             Verbose mode\n"
         << "    -w             Enable wide characters support\n"
         << "\n"
         << " Building stages options\n"
         << "    -n             Don't preprocess\n"
         << "    -p             Don't translate (stop after parsing)\n"
         << "    -E             Don't compile (stop after translation)\n"
         << "    -c             Don't make executable (stop after compilation)\n"
#if !defined(IRIX_CC) && !defined (_MSC_VER)
         << "    -P             Preprocess again after translation\n"
#endif
         << "    -m             Compile a metaclass (make a shared library)\n"
         << "\n"
         << " Preprocessor options\n"
         << "    -I<directory>  Add <directory> to the #include path\n"
         << "    -D<name>=<def> Define a macro <name> as <def>\n"
         << "    -d<option>     Specify a preprocessor option\n"
#if !defined(IRIX_CC) && !defined (_MSC_VER)
         << "    -C             Don't discard comments\n"
#endif
         << "\n"
         << " Other options\n"
         << "    --regular-c++  Inhibit the extended syntax\n";
}

static void ShowVersion()
{
    cerr << "occ (OpenC++) " << thisVersion << "\n\n" << copyrightNote << "\n";
}

static void PrintMetaclasses(const MetacompilerConfiguration& config)
{
    std::strstream buf;
    buf << "loaded metaclasses: ";
    TheMetaclassRegistry().PrintAllMetaclasses(buf);
    buf << std::ends;
    config.ErrorLog().Report(InfoMsg(buf.str()));
    buf.freeze(false);
}

static void LoadMetaclasses(const MetacompilerConfiguration& config)
{
    for(MetacompilerConfiguration::Iterator iter = config.Metaclasses();
    	! iter.AtEnd();
	iter.Advance())
    {
        string name(iter.Get());
        char* tmp = new char[name.length() + 1];
	std::copy(name.begin(), name.end(), tmp);
	tmp[name.length()] = '\0';
        DynamicMopMetaclassLoader().LoadMetaclass(tmp);
	delete tmp;
	// @warning All this new/delete/char* crap is because
	//     LoadMetaclass does not have 'const' on its argument
        //     Also vector<char> tmp(name.begin(),name.end())
	//     fails on gcc-2.95
    }
}    


int main(int argc, char** argv)
{
    CerrErrorLog errorLog;
    OpencxxConfiguration config(errorLog);

    try {
        ParseCommandLine(argc, argv, config);
    }
    catch (UnknownCliOptionException& e) {
        ShowHelp(e.GetName());
        std::exit(1);
    }
    
    if (config.ShowVersion()) {
        ShowVersion();
        std::exit(0);
    }
    
    Metaclass::SetConfiguration(&config);
    InitDynamicLoader(config.ErrorLog());
    LoadMetaclasses(config);
    
    if (config.PrintMetaclasses()) {
        PrintMetaclasses(config);
	std::exit(0);
    }

    try
    {
      Compile(config);
    }
    catch (const TooManyErrorsException &)
    {
      std::cerr << "to many errors" << std::endl;
      return -1;
    }
    catch (const FatalErrorException &)
    {
      std::cerr << "fatal error" << std::endl;
      return -1;
    }
    if (config.VerboseMode()) {
        errorLog.Report(InfoMsg("done."));
        if (! DONT_GC) 
        {
            strstream buf;
            buf << "GC happened " << GcCount() << " time(s)" << ends;
            errorLog.Report(InfoMsg(buf.str()));
            buf.freeze(false);
        }
    }
    
    Metaclass::SetConfiguration(0);
    ExitDynamicLoader();
    return 0;
}
