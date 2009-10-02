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

#include <opencxx/driver2.h>

#if defined(IRIX_CC)
// for open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <opencxx/MetacompilerConfiguration.h>

#include <ltdl.h>	// libtool dlopen library

using namespace std;

namespace Opencxx
{

// g++ recognizes a .ii file as preprocessed C++ source code.
// CC recognizes a .i file as preprocessed C++ source code.

#if defined(IRIX_CC)
#define OUTPUT_EXT	".i"
#else
#define OUTPUT_EXT	".ii"
#endif

#define CPP_EXT		".occ"
#define SLIB_EXT	".so"
#define OBJ_EXT		".o"

extern "C" {
#if !defined(IRIX_CC) && !defined(__GLIBC__) && !defined(__STRICT_ANSI__)
    int execvp(...);
#endif
    int wait(int*);
}

#if defined(IRIX_CC)
const char* compilerName = "CC";
#else
const char* compilerName = "g++";
#endif
const char* linkerName = "ld";
const char* opencxxErrorMessage = " Error(s).  OpenC++ stops.\n";

bool ParseTargetSpecificOptions(char* arg, char*& source_file);
void* LookupSymbol(lt_dlhandle handle, const char* symbol);

#if !SHARED_OPTION
static void RunSoLinker(const char* org_src, char* target);
#endif
static char* MakeTempFilename(const char* src, const char* suffix);


bool ParseTargetSpecificOptions(char*, char*&)
{
    return false;
}

void ExecVp(const char* file, char *const argv[])
{
    static const int verbose = true;
    if (verbose) {
        cerr << "EXECUTING: " << file;
        int i = 0;
        while (argv[i]) {
            cerr << " " << argv[i];
            ++i;
        }
        cerr << endl;
    }
    execvp(file, argv);
}

static void ShowCommandLine(const char*, const char** args)
{
    while(*args != 0)
        cerr << ' ' << *args++;
}



static void ShowCommandLine(const vector<string>& argv)
{
    for(unsigned int i = 0; i < argv.size(); ++i) {
        if (i) cerr << ' ';
        cerr << argv[i];
    }
    cerr << endl;
}

void ExecVp(const string& file, std::vector<std::string>& argv)
{
    using std::vector;
    using std::string;
    
    static const int verbose = true;
    if (verbose) {
        cerr << "EXECUTING: " << file;
        for(unsigned int i = 0; i < argv.size(); ++i)
        {
            cerr << " " << argv[i];
        }
        cerr << endl;
    }
    vector<char*> hardcoreArgv;
    for(vector<string>::iterator iter = argv.begin(),
                                 end  = argv.end();
        iter != end;
        ++iter)
    {
        hardcoreArgv.push_back(strdup((*iter).c_str()));
    }
    hardcoreArgv.push_back(0);
    execvp(file.c_str(), &*(hardcoreArgv.begin()));
}

void RunLinker(const MetacompilerConfiguration& config)
 // @todo Most likely we do not need to add options globally,
 //       so in fact argv.push_back() is sufficient,
 //       we don't need config.AddCcOption()
{
    if (!config.DoCompile() || !config.MakeExecutable()) {
	cerr << "OpenC++: no source file.\n";
	return;
    }

    string compiler(config.CompilerCommand());
    vector<string> argv;
    
    argv.push_back("");

    MetacompilerConfiguration::Iterator iter = config.CcOptions();
    while (! iter.AtEnd()) {
        argv.push_back(iter.Get());
        iter.Advance();
    }

    string linker = config.CompilerCommand();
    char* slib = 0;
    if (config.MakeSharedLibrary()) {
#if SHARED_OPTION
#if ((defined __APPLE__) && (defined __MACH__))
	argv.push_back("-dynamiclib");
	argv.push_back("-flat_namespace");
	argv.push_back("-undefined");
	argv.push_back("suppress");
#else
#if defined(IRIX_CC)
	argv.push_back("-n32");
#else
	argv.push_back("-fPIC");
#endif
	argv.push_back("-shared");
#endif
#else /* SHARED_OPTION */
	argv.push_back("-Bshareable");
	linker = config.LinkerCommand();
#endif
	if(config.SharedLibraryName() != "") {
	    slib = MakeTempFilename(config.SharedLibraryName().c_str(), SLIB_EXT);
	    argv.push_back("-o");
	    argv.push_back(slib);
	    delete [] slib;
	}
    }

    argv[0] = linker;

    if (config.VerboseMode()) {
	cerr << "[Link... ";
	ShowCommandLine(argv);
	cerr << "]\n";
    }

    if(fork() == 0){
	ExecVp(linker, argv);
	perror("cannot invoke a compiler");
    }
    else{
	int status;

	wait(&status);
	if(status != 0)
	    exit(1);
    }
}

char* RunPreprocessor(const char* src, 
    const MetacompilerConfiguration& config)
{
    char* dest = MakeTempFilename(src, CPP_EXT);
    string compiler = config.CompilerCommand();
    
    vector<string> argv;
    
    argv.push_back(compiler);

    if(config.RecognizeOccExtensions())
	argv.push_back("-D__opencxx");
    
    argv.push_back("-E");
#if defined(IRIX_CC)
    argv.push_back("-n32");
#else
    argv.push_back("-o");
    argv.push_back(dest);
    argv.push_back("-x");
    argv.push_back("c++");
#endif
    argv.push_back(src);

    if (config.VerboseMode()) {
	cerr << "[Preprocess... ";
	ShowCommandLine(argv);
#if defined(IRIX_CC)
	cerr << " > " << dest;
#endif
	cerr << "]\n";
    }

    if(fork() == 0){
#if defined(IRIX_CC)
	int fd = open(dest, O_WRONLY | O_CREAT, 0666);
	if (fd < 0) {
	    perror(dest);
	    exit(1);
	}
	dup2(fd, 1);
#endif
	ExecVp(compiler, argv);
	perror("cannot invoke a compiler");
    }
    else{
	int status;

	wait(&status);
	if(status != 0)
	    exit(1);
    }

    return dest;
}

char* OpenCxxOutputFileName(const char* src)
{
    return MakeTempFilename(src, OUTPUT_EXT);
}

/*
   To create a shared library foo.so from foo.cc,

   SunOS, Solaris, Linux (v2.0, gcc 2.7.2.2):
		g++ -fPIC -shared -o foo.so foo.cc

   Irix with naitive CC:
		CC -shared -n32 -o foo.so foo.cc

   FreeBSD:	g++ -fPIC -c foo.cc
		ld -Bshareable -o foo.so foo.o

*/
void RunCompiler(
    const char* org_src, const char* occ_src, 
    const MetacompilerConfiguration& config
)
{
    char* slib = 0;
    
    vector<string> argv;
    string compiler = config.CompilerCommand();
    argv.push_back(compiler);
    MetacompilerConfiguration::Iterator iter = config.CcOptions();
    while (! iter.AtEnd()) {
        argv.push_back(iter.Get());
        iter.Advance();
    }

    
    if (config.SharedLibraryName() != "") {

	slib = MakeTempFilename(config.SharedLibraryName().c_str(), SLIB_EXT);
#if SHARED_OPTION
#if ((defined __APPLE__) && (defined __MACH__))
	argv.push_back("-dynamiclib");
	argv.push_back("-flat_namespace");
	argv.push_back("-undefined");
	argv.push_back("suppress");
#else
#if defined(IRIX_CC)
	argv.push_back("-n32");
#else
	argv.push_back("-fPIC");
#endif
	argv.push_back("-shared");
#endif
	if (config.MakeExecutable()) {
	    argv.push_back("-o");
	    argv.push_back(slib);
	}
	else
	    argv.push_back("-c");
#else /* SHARED_OPTION */
	argv.push_back("-fPIC");
	argv.push_back("-c");
#endif
    }
    else
	if(! config.MakeExecutable())
	    argv.push_back("-c");

#if !defined(IRIX_CC)
    if (config.PreprocessTwice()) {
	argv.push_back("-x");
	argv.push_back("c++");
    }
#endif

    argv.push_back(occ_src);

    if (config.VerboseMode()) {
	cerr << "[Compile... ";
	ShowCommandLine(argv);
	cerr << "]\n";
    }

    if (fork() == 0) {
	ExecVp(compiler, argv);
	perror("cannot invoke a compiler");
    }
    else{
	int status;

	wait(&status);
	if(status != 0)
	    exit(1);
    }

#if !SHARED_OPTION
    if (config.MakeSharedLibrary() && config.MakeExecutable())
	RunSoLinker(org_src, slib);
#endif

    delete [] slib;
}

void RunSoCompiler(
    const char* src_file
  , const MetacompilerConfiguration& config
)
{
    const char* cc_argv[9];
    int i = 0;

    char* slib = MakeTempFilename(src_file, SLIB_EXT);
    cc_argv[i++] = compilerName;
#if SHARED_OPTION
#if ((defined __APPLE__) && (defined __MACH__))
	cc_argv[i++] = "-dynamiclib";
	cc_argv[i++] = "-flat_namespace";
	cc_argv[i++] = "-undefined";
	cc_argv[i++] = "suppress";
#else
#if defined(IRIX_CC)
    cc_argv[i++] = "-n32";
#else
    cc_argv[i++] = "-fPIC";
#endif
    cc_argv[i++] = "-shared";
#endif
    cc_argv[i++] = "-o";
    cc_argv[i++] = slib;
#else
    cc_argv[i++] = "-fPIC";
    cc_argv[i++] = "-c";
#endif
    cc_argv[i++] = src_file;
    cc_argv[i] = (char*)0;

    if (config.VerboseMode()) {
	cerr << "[Compile... ";
	ShowCommandLine(compilerName, cc_argv);
	cerr << "]\n";
    }

    if(fork() == 0){
	ExecVp(compilerName, (char**)cc_argv);
	perror("cannot invoke a compiler");
    }
    else{
	int status;

	wait(&status);
	if(status != 0)
	    exit(1);
    }

#if !SHARED_OPTION
    RunSoLinker(src_file, slib);
#endif

    delete [] slib;
}

// RunSoLinker() is used only if SHARED_OPTION is false (FreeBSD).
#if !SHARED_OPTION

static void RunSoLinker(const char* org_src, char* target)
{
    const char* ld_argv[6];
    ld_argv[0] = linkerName;
    ld_argv[1] = "-Bshareable";
    ld_argv[2] = "-o";
    ld_argv[3] = target;
    ld_argv[4] = MakeTempFilename(org_src, OBJ_EXT);
    ld_argv[5] = (char*)0;

    if(verboseMode){
	cerr << "[Link... ";
	ShowCommandLine(linkerName, ld_argv);
	cerr << "]\n";
    }

    if(fork() == 0){
	ExecVp(linkerName, (char**)ld_argv);
	perror("cannot invoke a linker");
    }
    else{
	int status;

	wait(&status);
	if(status != 0)
	    exit(1);
    }

    unlink(ld_argv[4]);
    delete [] ld_argv[4];
}
#endif /* SHARED_OPTION */

/*
   For example, if src is "../foo.cc", MakeTempFilename() makes
   "foo.<suffix>".
*/
static char* MakeTempFilename(const char* src, const char* suffix)
{
    const char* start;
    const char* end;

    start = strrchr(src, '/');
    if(start == 0)
	start = src;
    else
	++start;

    end = strrchr(start, '.');
    if(end == 0)
	end = src + strlen(src);

    char* result = new char[end - start + strlen(suffix) + 1];
    strncpy(result, start, end - start);
    result[end - start] = '\0';
    strcat(result, suffix);
    return result;
}

}
