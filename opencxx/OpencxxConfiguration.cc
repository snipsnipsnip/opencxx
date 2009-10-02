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

#include <opencxx/OpencxxConfiguration.h>
#include <cassert>
#include <cstdlib>
#include <opencxx/Class.h>
#include <opencxx/CliErrorMsg.h>
#include <opencxx/GenericMsg.h>
#include <opencxx/InfoMsg.h>
#include <opencxx/UnknownCliOptionException.h>
#include "opencxx/parser/CerrErrorLog.h"
#include <opencxx/parser/ErrorLog.h>

namespace Opencxx
{

/**
  @todo Figure out who owns the <code>option</code> c-string
        and make this method side-effect free.
  */
void OpencxxConfiguration::RecordCmdOption(char* option)
{
    if(option == 0 || *option == '\0')
        return;

    char* value = strchr(option, '=');
    if(value != 0)
        *value++ = '\0';

    if(!Class::RecordCmdLineOption(option, value)){
        errorLog_.Report(CliErrorMsg("too many `-M' options"));
        assert(! "CliErrorMsg should throw");
    }
}

static bool streq(const char* s1, const char* s2)
{
    return std::strcmp(s1, s2) == 0;
}

static bool strpref(const char* prefix, const char* text)
{
    if (strlen(prefix) > strlen(text)) return false;
    return strncmp(prefix, text, strlen(prefix)) == 0;
}

/*
  File Naming convention

  .c, .C, .cc, .cpp, .cxx : C++ source files
  .mc : metaclass files
  .occ, .ii, .i (IRIX_CC): temporary files used by OpenC++
*/
static bool IsCxxSource(const char* fname)
{
    char* ext = strrchr(fname, '.');
    if(ext == 0)
        return false;

    if(strcmp(ext,    ".cc") == 0
       || strcmp(ext, ".C") == 0
       || strcmp(ext, ".c") == 0
       || strcmp(ext, ".mc") == 0
       || strcmp(ext, ".cxx") == 0
       || strcmp(ext, ".cpp") == 0
       || strcmp(ext, ".ii") == 0
       || strcmp(ext, ".i") == 0
       || strcmp(ext, ".occ") == 0)
        return true;

    return false;
}

void ParseCcOptions(
    const std::string& arg
  , OpencxxConfiguration& config)
{
    if(arg[0] != '-') {
        if(IsCxxSource(arg.c_str()))
            if (config.SourceFileName() == "") {
                config.SetSourceFileName(arg);
                return;
            }
            else {
                config.ErrorLog().Report(
                    GenericMsg(
                        Msg::Fatal, SourceLocation(), 
                        "multiple source files not allowed"
                    )
                );
                assert(! "fatal message should throw");
            }
    }

    config.AddObjectFile();
    if (config.SourceFileName() == "") {
        config.AddCcOption(arg);
    }
    else {
        config.AddCc2Option(arg);
    }
}



void ParseCommandLine(int argc, char** argv, OpencxxConfiguration& config)
    /* throws UnknownCliOption */
{
    int i;
    for(i = 1; i < argc; ++i) {
        if (streq("--", argv[i])) break;
        else if(streq("--private--external-driver", argv[i])) {
            config.SetExternalDriver(true);
        }
        else if(streq("--private--libtool-plugins", argv[i])) {
            config.SetLibtoolPlugins(true);
        }
        else if(streq("--private--no-static-initialization", argv[i])) {
            config.SetStaticInitialization(false);
        }
        else if (streq("-E", argv[i])) config.SetDoCompile(false);
        else if (streq("--private--output", argv[i])) {
            ++i;
            if (argc <= i) {
                config.ErrorLog().Report(CliErrorMsg("missing argument to `--output'"));
                assert(! "CLI error should throw");
            }
            else {
                config.SetOutputFileName(argv[i]);
            }
        }
        else if (streq("-g", argv[i])) config.AddCcOption(argv[i]);
        else if (streq("-n", argv[i])) config.SetDoPreprocess(false);
        else if (strpref("-m", argv[i])) {
            config.SetMakeSharedLibrary(true);
            config.SetSharedLibraryName(&argv[i][2]);
        }
        else if (streq("-P", argv[i])) config.SetPreprocessTwice(true);
        else if (streq("-p", argv[i])) config.SetDoTranslate(false);
        else if (streq("-C", argv[i])) {
            config.AddCppOption("-C");
            config.SetPreprocessTwice(true);
        }
        else if(streq("-c", argv[i])) config.SetMakeExecutable(false);
        else if(streq("-l", argv[i])) {
	    config.SetPrintMetaclasses(true);
        }
        else if(strpref("-S", argv[i])) {
            if (argv[i][2] == 0) {
	        config.AddMetaclass(argv[++i]);
            }
            else {
	        config.AddMetaclass(&argv[i][2]);
            }
        }
        else if(streq("-s", argv[i])) config.SetShowProgram(true);
        else if(streq("-v", argv[i])) config.SetVerboseMode(true);
        else if(streq("-V", argv[i])) {
            config.SetShowVersion(true);
            return;
        }
        else if (streq("--regular-c++", argv[i]))
            config.SetRecognizeOccExtensions(false);
        else if (strpref("-D", argv[i])) config.AddCppOption(argv[i]);
        else if (strpref("-I", argv[i])) config.AddCppOption(argv[i]);
        else if (strpref("-d", argv[i]) && !streq("-d", argv[i])) {
            config.AddCppOption(&argv[i][2]);
        }
        else if (strpref("-M", argv[i])) config.RecordCmdOption(&argv[i][2]);
        else if (streq("-w", argv[i])) config.SetWcharSupport(true);
        else if (strpref("-", argv[i])) {
            throw UnknownCliOptionException(argv[i]);
        }
        else ParseCcOptions(argv[i], config);
    }
    while (++i < argc) {
        ParseCcOptions(argv[i], config);
    }
    
    if (! config.DoTranslate()) {
        config.SetDoCompile(false);
    }
}

}


