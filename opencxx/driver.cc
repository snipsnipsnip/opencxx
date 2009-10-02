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

#include <opencxx/driver.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <strstream>
#include <cerrno>
#include <opencxx/DynamicMopMetaclassLoader.h>
#include "opencxx/parser/CerrErrorLog.h"
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/GenericMsg.h>
#include <opencxx/InfoMsg.h>
#include <opencxx/parser/Lex.h>
#include <opencxx/TheMetaclassRegistry.h>
#include <opencxx/MetacompilerConfiguration.h>
#include <opencxx/parser/Parser.h>
#include <opencxx/parser/Program.h>
#include <opencxx/parser/ProgramFile.h>
#include <opencxx/parser/ProgramFromStdin.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/Walker.h>
#include <opencxx/parser/auxil.h>
#include <opencxx/classwalk.h>
#include <opencxx/driver2.h>
#include <opencxx/parser/Ptree.h>

#if defined(_MSC_VER)
#include <stdlib.h>
#else
#include <unistd.h>
#endif

using namespace std;

namespace Opencxx
{

const int NARGS = 64;
const char* cppArgv[NARGS];
const char* ccArgv[NARGS];
/*static const char* cc2Argv[NARGS];*/      // arguments following a source file
int cppArgc = 0;
int ccArgc = 0;
//static int cc2Argc = 0;
//static int numOfObjectFiles = 0;

// defined in driver2.cc
//extern bool ParseTargetSpecificOptions(char* arg, char*& source_file);
//extern void RunLinker();
//extern char* RunPreprocessor(const char* src, bool regularCpp);
//extern char* OpenCxxOutputFileName(const std::string& src);
//extern void RunCompiler(const char* src, const char* occserc);

// defined in metaclass.cc
//extern void LoadMetaclass(char*);

static void ProcessStdin(ostream&, const MetacompilerConfiguration&);
static void ReadFile(const char* src, const MetacompilerConfiguration&);
static string RunOpencxx(const std::string& src, const MetacompilerConfiguration&);
static void ParseOpencxx(Program* parse, const MetacompilerConfiguration&);


/* void Compile(int argc, char** argv)  */
void Compile(const MetacompilerConfiguration& config)
{
    string src(config.SourceFileName());
    if (src != "")
    {
        ReadFile(src.c_str(), config);
    }
    else
    {
        if(config.NumOfObjectFiles() == 0)
        {
            if (config.OutputFileName() != "")
            {
                ofstream output(config.OutputFileName().c_str());
                if (!output)
                {
                    std::strstream buf;
                    buf << "cannot open `" << config.OutputFileName() 
                        << "' for writing" << ends;
                    config.ErrorLog().Report(
                        GenericMsg(Msg::Fatal, SourceLocation(), buf.str())
                    );
                    buf.freeze(false);
                    assert(! "fatal message should throw");
                }
                ProcessStdin(output, config);
                output.close();
            }
            else
            {
                ProcessStdin(cout, config);
            }
        }
        else
        {
            RunLinker(config);
        }
    }
}

static void ProcessStdin(ostream& os, const MetacompilerConfiguration& config)
{
    ProgramFromStdin prog;
    try
    {
        ParseOpencxx(&prog, config);
    }
    catch (const TooManyErrorsException& e) // :TODO: elevate?
    {
        cerr << "occ: too many errors" << endl;
        exit(1);
    }
    prog.Write(os, "stdout");
    Class::FinalizeAll(os);
    TheMetaclassRegistry().FinalizeAll(os);
}

static void ReadFile(
    const char* src, 
    const MetacompilerConfiguration& config)
{
    //bool regularCpp = ! FIXME
    config.RecognizeOccExtensions();

    std::string occfile;
    if (config.DoPreprocess()) {
        std::string cppfile = RunPreprocessor(src, config);
        occfile = RunOpencxx(cppfile, config);
        unlink(cppfile.c_str());
    }
    else {
        occfile = RunOpencxx(src, config);
    }

    if (config.DoCompile()) {
        RunCompiler(src, occfile.c_str(), config);
    }
}

static string RunOpencxx(
    const std::string& src, 
    const MetacompilerConfiguration& config
)
{
    char* dest = OpenCxxOutputFileName(src.c_str());

    ifstream src_stream(src.c_str());
    if (!src_stream){
        strstream buf;
        buf << "cannot open `" << src << "' for reading"
            << " (" << strerror(errno) << ")" << ends;
        config.ErrorLog().Report(
            GenericMsg(Msg::Fatal, SourceLocation(), buf.str())
        );
        buf.freeze(false);
        assert(! "fatal message should throw");
    }

    ProgramFile src_prog(src_stream);
    if (config.VerboseMode()) {
        if (config.DoTranslate()) {
            strstream buf;
            buf << "translating `" << src << "' into `" << dest << "'" << ends;
            config.ErrorLog().Report(InfoMsg(buf.str()));
            buf.freeze(false);
        }
        else {
            strstream buf;
            buf << "parsing `" << src << "'" << ends;
            config.ErrorLog().Report(InfoMsg(buf.str()));
            buf.freeze(false);
        }
    }

    try
    {
        ParseOpencxx(&src_prog, config);
    }
    catch (const TooManyErrorsException& e)
    {
        config.ErrorLog().Report(
            GenericMsg(Msg::Fatal, SourceLocation(), "too many errors")
        );
        assert(! "fatal message should throw");
    }

    if (config.DoTranslate()) {
        ofstream dest_stream(dest, (ios::out | ios::trunc));
        if (!dest_stream) {
            strstream buf;
            buf << "cannot open `" << dest << "' for writting"
                << " (" << strerror(errno) << ")" << ends;
            config.ErrorLog().Report(
                GenericMsg(Msg::Fatal, SourceLocation(), buf.str())
            );
            buf.freeze(false);
            assert(! "fatal message should throw");
        }

        src_prog.Write(dest_stream, dest);
        Class::FinalizeAll(dest_stream);
        TheMetaclassRegistry().FinalizeAll(dest_stream);
    }

    src_stream.close();
    return dest;
}

static void ParseOpencxx(Program* prog, const MetacompilerConfiguration& config) 
/* throws TooManyErrorsException */
{
    Lex lexer(prog, config.WcharSupport(), config.RecognizeOccExtensions());
    Parser parser(&lexer, config.ErrorLog());
    ClassWalker w(&parser);
    Ptree* def;

    parser.InstallMetaclassLoader(new DynamicMopMetaclassLoader());

    while (parser.rProgram(def)) {

        if (config.ShowProgram()) {
            def->Display(cout);
        }

        if (config.DoTranslate()) {
            Ptree* def2 = w.Translate(def);
            Ptree* before = w.GetInsertedPtree();
            Ptree* after = w.GetAppendedPtree();

            prog->Insert(def, before, after);
            if (def != def2) {
                prog->MinimumSubst(def2, def);
            }
        }
    }

    if (parser.SyntaxErrorsFound()) {
        config.ErrorLog().Report(
            GenericMsg(
                Msg::Fatal, SourceLocation(), 
                "parse errors, exiting"
            )
        );
        assert(! "fatal error should throw");
    }
    

}

}
