//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{xerox}{}@
//@license{contributors}{}@
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
//  -----------------------------------------------------------------
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

#include <strstream>
#include <iostream>
#include <string>
#include <opencxx/Metaclass.h>
#include <opencxx/TheMetaclassRegistry.h>
#include <opencxx/driver2.h>
#include <opencxx/MetacompilerConfiguration.h>

#if USE_DLOADER
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <cstdlib>	/* for exit() */
#include <opencxx/defs.h>
#include <opencxx/dloading.h>
using namespace std;

namespace Opencxx
{

#if !USE_SO
//extern void BaseClassUsed(char *, int);		// in driver2.cc
#endif /* USE_SO */

#endif /* USE_DLOADER */


// class QuoteClass
//
// Part of the implementation of QuoteClass is included here to let
// quote-class.cc linked to others if the compiler is produced by
// "g++ -o occ opencxx.a".

}

#include <opencxx/QuoteClass.h>

using namespace std;

namespace Opencxx
{


// class Metaclass

MetacompilerConfiguration* Metaclass::s_metacompilerConfiguration_ = 0;



Metaclass::Metaclass()
{
    new_function_name = 0;
    first_not_inlined_vf = -1;
}

bool Metaclass::Initialize()
{
    return true;
}

char* Metaclass::MetaclassName()
{
    return "Metaclass";
}

void Metaclass::TranslateClass(Environment* env)
{
    Ptree* name = Name();

    if(!IsBuiltinMetaclass(name)){
	CheckObsoleteness();
	InsertInitialize();
#if defined(_MSC_VER) || defined(_PARSE_VCC)
	AddClassSpecifier(Ptree::Make("__declspec(dllexport)"));
#endif
	AppendMember(Ptree::Make("public: char* MetaclassName() {\n"
				 "    return \"%p\"; }\n",
				 Name()));

	Ptree* tmpname = PtreeUtil::GenSym();
	Ptree* tmpname2 = PtreeUtil::GenSym();
	Ptree* finalizer = GetFinalizer();
	AppendAfterToplevel(env, Ptree::Make(
		"static Class* %p(Ptree* def, Ptree* marg)\n"
		"{\n"
		"    Class* c = new %p;\n"
		"    c->InitializeInstance(def, marg);\n"
		"    return c;\n"
		"}\n"
		"\n",
		tmpname, name));
        if (s_metacompilerConfiguration_->StaticInitialization())
        {
            AppendAfterToplevel(env, Ptree::Make(
                    "static MetaclassRegistration %p(\"%p\", %p,\n"
                    "    %p::Initialize, %p);\n"
                    "\n",
                    tmpname2, name, tmpname, name, finalizer));
        }
	if(s_metacompilerConfiguration_->MakeSharedLibrary()){
	    ProduceInitFile(name);
	    first_not_inlined_vf = FindFirstNotInlinedVirtualFunction();
	    new_function_name = tmpname;
	    if(first_not_inlined_vf < 0)
		AppendHousekeepingCode(env, Name(), tmpname, finalizer);
	}
    }

    Class::TranslateClass(env);
}

Ptree* Metaclass::GetFinalizer()
{
    Member m;
    if(LookupMember("FinalizeClass", m) && m.Supplier() == this){
        if(!m.IsStatic())
	    ErrorMessage("FinalizeClass() must be static in ", Name(),
			 Definition());

	return Ptree::Make("%p::FinalizeClass", Name());
    }
    else
	return Ptree::Make("0");
}

void Metaclass::CheckObsoleteness()
{
    Member m;

    if(LookupMember("Finalize", m) && m.Supplier() == this)
	WarningMessage("Finalize() is obsolete.  Use FinalizeInstance() in ",
		       Name(),
		       Definition());
}

// Thanks to http://www.eskimo.com/~scs/C-faq/q11.17.html for this
#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

void Metaclass::ProduceInitFile(Ptree* class_name)
{
#if USE_DLOADER
#if USE_SO
    const char* fname = Ptree::Make("%p-init.cc", class_name)->ToString();
    if(s_metacompilerConfiguration_->VerboseMode())
	std::cerr << "Produce " << fname << " ..\n";

    ofstream src_file(fname);
    if(!src_file){
        perror(fname);
	exit(1);
    }

    src_file << STRINGIFY(BEGIN_OPENCXX_NAMESPACE) << "\n";
    src_file << "extern void LoadMetaclass(char*);\n";
    src_file << STRINGIFY(END_OPENCXX_NAMESPACE) << "\n";
    src_file << "extern \"C\" {";
    src_file << "char* opcxx_init_" << class_name << "(){\n";

    Ptree* base_name;
    for(int i = 0; (base_name = NthBaseClassName(i)) != 0; ++i)
	if(!PtreeUtil::Eq(base_name, "Class"))
	    src_file << "  " << STRINGIFY(OPENCXX_NAMESPACE) << "::LoadMetaclass(\"" << base_name << "\");\n";

    src_file <<	"    return 0;\n"
             << "}\n"
             << "}\n";

    src_file.close();

    if (s_metacompilerConfiguration_->ExternalDriver()) 
    {
        std::cerr 
            << "#attn-external-driver compile-plugin-init " 
            << fname << " " << class_name << "-init" << endl;
        std::cerr 
            << "#attn-external-driver plugin-name " 
            << class_name << endl;
    }
    else
    {
        RunSoCompiler(fname, *s_metacompilerConfiguration_);
    }
#else
    // Push base class names forward to RunCompiler
    Ptree* base_name;
    for (int i = 0; (base_name = NthBaseClassName(i)) != 0; ++i)
	if (!base_name->Eq("Class") && !base_name->Eq("TemplateClass"))
	    BaseClassUsed(base_name->GetPosition(), base_name->GetLength());
#endif /* USE_SO */
#endif /* USE_DLOADER */
}

bool Metaclass::IsBuiltinMetaclass(Ptree* name)
{
    using PtreeUtil::Eq;
    return bool(Eq(name, "Class") || Eq(name, "Metaclass")
		|| Eq(name, "TemplateClass")
		|| Eq(name, "QuoteClass"));
}

void Metaclass::InsertInitialize()
{
    Member m;
    if(!LookupMember("Initialize", m) || m.Supplier() != this){
#if !defined(_MSC_VER) || (_MSC_VER >= 1100)
	AppendMember(Ptree::Make(
		"public: static bool Initialize() { return 1; }\n"));
#else
	AppendMember(Ptree::Make(
		"public: static int Initialize() { return 1; }\n"));
#endif
    }
    else if(!m.IsStatic())
	ErrorMessage("Initialize() must be static in ", Name(),
		     Definition());
}

int Metaclass::FindFirstNotInlinedVirtualFunction()
{
    Member m;
    for(int i = 0; NthMember(i, m); ++i)
	if(m.IsFunction() && m.IsVirtual() && !m.IsInline()
	   && m.Supplier() == this)
	    return i;

    WarningMessage("a metaclass should include at least one"
		   " not-inlined virtual function: ", Name(), Name());
    return -1;
}

void Metaclass::TranslateMemberFunction(Environment* env, Member& m)
{
    if(m.Nth() != first_not_inlined_vf)
	return;

    if(m.IsInline()){
	ErrorMessage("This member function should not be inlined: ",
		     m.Name(), m.ArgumentList());
	return;
    }

    AppendHousekeepingCode(env, Name(), new_function_name,
			   GetFinalizer());
}

void Metaclass::AppendHousekeepingCode(Environment* env, Ptree* class_name,
				       Ptree* creator_name,
				       Ptree* finalizer)
{
#if !defined(_MSC_VER)
    AppendAfterToplevel(env, Ptree::Make(
                "extern \"C\" {\n"
		"void opcxx_register_%p()\n"
		"{\n"
		"    TheMetaclassRegistry().Add(\"%p\", %p,\n"
		"        %p::Initialize, %p);\n"
		"}\n"
		"}\n",
		class_name, class_name, creator_name, class_name,
		finalizer));
#endif
}

//void LoadMetaclass(char* metaclass_name)
//{
//#if USE_DLOADER
//    //cerr << "[LoadMetaclass(" << metaclass_name << ")]" << endl;
//    if(metaclass_name != 0 && *metaclass_name != '\0')
//	if(!TheMetaclassRegistry().AlreadyRecorded(metaclass_name))
//	    Metaclass::Load(metaclass_name, strlen(metaclass_name));
//#endif
//}

void Metaclass::Load(Ptree* metaclass_name)
{
#if USE_DLOADER
    if(TheMetaclassRegistry().AlreadyRecorded(metaclass_name))
	return;

    Load(metaclass_name->GetPosition(), metaclass_name->GetLength());
#endif
}

static string oss_to_string(ostrstream &os)
{
  string s;
  s.append(os.str(), os.pcount());
  os.rdbuf()->freeze(false);
  return s;
}

static string generate_symbol(const char *func_name, const char *metaclass_name)
{
#if 0
  string sym = string(func_name) + "_" + metaclass_name;
  ostrstream os;
  os << sym.size();
  return string("_Z") + oss_to_string(os) + sym + "v";
#endif
  return string(func_name) + "_" + metaclass_name;
}

void Metaclass::Load(const char* mcn, int len)
{
    if (s_metacompilerConfiguration_->LibtoolPlugins())
    {
        string metaclass_name(mcn, len);
        string file_name = metaclass_name + "-init.la";

        lt_dlhandle handle_init = LoadSoLib(file_name.c_str()); // load <metaclass>-init.la

        // call opcxx_init_<metaclass>() in <metaclass>-init.la

        string func_name = generate_symbol("opcxx_init", metaclass_name.c_str());
        void (*loader)();
        loader = (void (*)())LookupSymbol(handle_init, func_name.c_str());
        (*loader)();

        file_name = metaclass_name + ".la";
        lt_dlhandle handle = LoadSoLib(file_name.c_str());	// load <metaclass>.la

        if (s_metacompilerConfiguration_->VerboseMode()) std::cerr << "Initialize.. ";

        // call opcxx_init_<metaclass>() in <metaclass>.la

        func_name = generate_symbol("opcxx_register", metaclass_name.c_str());
        void (*func)();
        func = (void (*)())LookupSymbol(handle, func_name.c_str());
        (*func)();

        if(s_metacompilerConfiguration_->VerboseMode()) std::cerr << "Done.\n";
        return;
    }
}

lt_dlhandle Metaclass::LoadSoLib(const char* file_name)
{
    lt_dlhandle handle = 0;
#if USE_DLOADER
    if(s_metacompilerConfiguration_->VerboseMode())
	std::cerr << "Load " << file_name << ".. ";

    handle = OPENCXX_NAMESPACE_QUAL(LoadSoLib)(file_name);
#endif /* USE_DLOADER */

    return handle;
}

void* Metaclass::LookupSymbol(lt_dlhandle handle, const char* symbol)
{
    void* func = 0;
#if USE_DLOADER
    func = OPENCXX_NAMESPACE_QUAL(LookupSymbol)(handle, symbol);
#endif
    return func;
}

}

