#ifndef guard_opencxx_MetaclassRegistry_h
#define guard_opencxx_MetaclassRegistry_h

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

#include <map>
#include <string>
#include <iosfwd>
#include <opencxx/defs.h>

namespace Opencxx
{
    class Ptree;
    class Class;

    class MetaclassRegistry
    {
    public:
        typedef Class* (Creator)(Ptree*,Ptree*);
        typedef bool (Initializer)();
        typedef Ptree* (Finalizer());
        
    private:

        struct Record
        {
            Record(Creator* c, Initializer* i, Finalizer* f)
              : creator_(c), initializer_(i), finalizer_(f)
            {
            }
            
            Creator*     creator_;
            Initializer* initializer_;
            Finalizer*   finalizer_;
        };

        typedef std::map<std::string,Record> MetaclassContainer;

    public:
    
        MetaclassRegistry() {}

        void Add(const std::string& name, 
            Creator* ctor, Initializer* izer, Finalizer* fzer);
        Class* New(const char* name, Ptree* def, Ptree* marg);
        Class* New(Ptree* name, Ptree* def, Ptree* marg);
        // :TODO: Get rid of this method, let the clients call ToString().
        //    to eliminate dependency on Ptree in implementation
        void FinalizeAll(std::ostream& out);
        bool AlreadyRecorded(const char* name);
        // :TODO: Get rid of this method, let the clients call ToString()
        //    to eliminate dependency on Ptree in implementation
        bool AlreadyRecorded(Ptree* name);
        void PrintAllMetaclasses(std::ostream&);
    private:
        void operator=(const MetaclassRegistry&);
        MetaclassRegistry(const MetaclassRegistry&);

    private:

        MetaclassContainer metaclasses_;
    };
    
}

#endif /* ! guard_opencxx_MetaclassRegistry_h */
