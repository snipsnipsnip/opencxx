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
#include <opencxx/MetaclassRegistry.h>
#include <opencxx/parser/Ptree.h>
#include "opencxx/parser/CerrErrorLog.h"
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/MopMsg.h>

namespace Opencxx
{

using namespace std;

void
MetaclassRegistry::Add(const string& name, 
    Creator* ctor, Initializer* izer, Finalizer* fzer)
{
    //cerr << "[MetaclassRegistry::Add(" << name << ")]" << endl;
    metaclasses_.insert(make_pair(name,Record(ctor,izer,fzer)));
    assert(izer);
    if (! izer())
    {
        TheErrorLog().Report(MopMsg(Msg::Fatal, "Initialize()", "the initialization process failed."));
    }
}

Class* 
MetaclassRegistry::New(const char* name, Ptree* def, Ptree* marg)
{
    // :TODO: assert(name)
    if (name)
    {
        string n(name);
        MetaclassContainer::iterator iter = metaclasses_.find(n);
        if (iter != metaclasses_.end())
        {
            return (iter->second.creator_)(def, marg);
        }
    }
    return 0;
}

Class* 
MetaclassRegistry::New(Ptree* name, Ptree* def, Ptree* marg)
{
    // :TODO: assert(name)
    return New(name->ToString(), def, marg);
}

void 
MetaclassRegistry::FinalizeAll(ostream& out)
{
    // :TODO: factor out the 'Write' functionality, e.g.
    //     by acceptin not ostream&, but pointer to function
    //     or pointer to iface class
    
    for(MetaclassContainer::iterator iter = metaclasses_.begin(),
                                     end  = metaclasses_.end();
        iter != end;
        ++iter)
    {
        if (iter->second.finalizer_)
        {
            Ptree* code = (iter->second.finalizer_)();
            if (code)
            {
                code->Write(out);
                out << "\n";
            }
        }
    }
}

bool 
MetaclassRegistry::AlreadyRecorded(const char* name)
{
    return metaclasses_.find(string(name)) != metaclasses_.end();
}

bool 
MetaclassRegistry::AlreadyRecorded(Ptree* name)
{
    return AlreadyRecorded(name->ToString());
}

void 
MetaclassRegistry::PrintAllMetaclasses(ostream& os)
{
    for(MetaclassContainer::iterator iter = metaclasses_.begin(),
                                     end  = metaclasses_.end();
        iter != end;
        ++iter)
    {
        os << iter->first << " ";
    }
}

}
