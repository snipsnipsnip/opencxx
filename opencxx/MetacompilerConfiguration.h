#ifndef guard_opencxx_MetacompilerConfiguration_h
#define guard_opencxx_MetacompilerConfiguration_h

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

#include <opencxx/defs.h>
#include <string>
#include <memory>

namespace Opencxx
{

class ErrorLog;

class MetacompilerConfiguration
{
protected:
    class IteratorIface
    {
    public:
        virtual std::string Get() const = 0;
        virtual bool AtEnd() const = 0;
        virtual void Advance() = 0;
        virtual std::auto_ptr<IteratorIface> Clone() const = 0;
    };

public:    
    class Iterator
    {
    public:
        template <class T>
        Iterator(std::auto_ptr<T> impl) : impl_(impl) {}
        Iterator(const Iterator& iter) : impl_(iter.impl_->Clone()) {}
        Iterator& operator=(Iterator iter)
        {
            std::swap(impl_, iter.impl_);
            return *this;
        }
        std::string Get() const { return impl_->Get(); }
        bool        AtEnd() const { return impl_->AtEnd(); }
        void        Advance() { return impl_->Advance(); }
    private:
        std::auto_ptr<IteratorIface> impl_;
    };

    /** True iff running in verbose mode */ 
    virtual bool VerboseMode() const = 0;

    virtual bool LibtoolPlugins() const = 0;
    
    /** True iff compiling to dlopenable module */
    virtual bool MakeSharedLibrary() const = 0;
    
    virtual std::string SharedLibraryName() const = 0;
    
    /** True iff occ is run by external driver (occ2) */
    virtual bool ExternalDriver() const = 0;
    
    virtual bool StaticInitialization() const = 0;
    
    virtual bool DoCompile() const = 0;
    virtual bool DoTranslate() const = 0;
    
    virtual bool DoPreprocess() const  = 0;
    virtual bool PreprocessTwice() const = 0;
    virtual bool MakeExecutable() const = 0;
    virtual bool RecognizeOccExtensions() const = 0;
    virtual bool WcharSupport() const = 0;
    virtual bool PrintMetaclasses() const = 0;
    
    virtual std::string SourceFileName() const = 0;
    virtual std::string OutputFileName() const = 0;
    virtual int NumOfObjectFiles() const = 0;
    
    virtual std::string CompilerCommand() const = 0;
    virtual std::string PreprocessorCommand() const = 0;
    virtual std::string LinkerCommand() const = 0;
    
    virtual bool ShowProgram() const = 0;
                
    virtual Opencxx::ErrorLog& ErrorLog() const = 0;
    
    virtual Iterator CcOptions() const = 0;
    
    virtual Iterator Metaclasses() const = 0;
};

}

#endif /* ! guard_opencxx_MetacompilerConfiguration_h */
