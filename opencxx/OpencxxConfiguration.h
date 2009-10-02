#ifndef guard_opencxx_OpencxxConfiguration_h
#define guard_opencxx_OpencxxConfiguration_h
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

#include <vector>
#include <string>
#include <opencxx/defs.h>
#include <opencxx/MetacompilerConfiguration.h>

namespace Opencxx
{

class ErrorLog;

class OpencxxConfiguration : public MetacompilerConfiguration
{
private:
    class IteratorImpl : public MetacompilerConfiguration::IteratorIface
    {
    public:
        IteratorImpl(std::vector<std::string>::const_iterator begin,
                     std::vector<std::string>::const_iterator end)
          : iter_(begin)
          , end_(end)
        {
        }
        
        std::string Get() const { return *iter_; }
        void Advance() { ++iter_; }
        bool AtEnd() const { return iter_ == end_; }
        std::auto_ptr<MetacompilerConfiguration::IteratorIface> Clone() const
        {
            return std::auto_ptr<MetacompilerConfiguration::IteratorIface>(
                new IteratorImpl(*this)
            );
        }
        
    private:
        IteratorImpl(const IteratorImpl& that)
          : iter_(that.iter_)
          , end_(that.end_)
        {
        }
        
        void operator=(const IteratorImpl&);
        
        std::vector<std::string>::const_iterator iter_;
        std::vector<std::string>::const_iterator end_;
    };
    
public:
    OpencxxConfiguration(Opencxx::ErrorLog& errorLog)
      : errorLog_(errorLog)
      , verboseMode_(false)
      , libtoolPlugins_(false)
      , doPreprocess_(true)
      , preprocessTwice_(false)
      , makeExecutable_(true)
      , makeSharedLibrary_(false)
      , recognizeOccExtensions_(true)
      , wcharSupport_(false)
      , staticInitialization_(true)
      , doCompile_(true)
      , showProgram_(false)
      , externalDriver_(false)
      , doTranslate_(true)
      , showVersion_(false)
      , printMetaclasses_(false)
      , numOfObjectFiles_(0)
      , sharedLibraryName_()
      , compilerCommand_("g++")
      , preprocessorCommand_("g++")
      , linkerCommand_("g++")
    {
    }

    virtual ~OpencxxConfiguration() {}

    void SetVerboseMode(bool v) { verboseMode_ = v; }
    bool VerboseMode() const { return verboseMode_; }
    
    void SetLibtoolPlugins(bool v) { libtoolPlugins_ = v; }
    bool LibtoolPlugins() const { return libtoolPlugins_; }
    void AddCcOption(const std::string& option)
    {
        ccOptions_.push_back(option);
    }
    
    Iterator CcOptions() const
    {
        return Iterator(
            std::auto_ptr<IteratorImpl>(
                new IteratorImpl(ccOptions_.begin(), ccOptions_.end())
            )
        );
    }
    
    void AddCc2Option(const std::string& option)
    {
        cc2Options_.push_back(option);
    }
    void AddCppOption(const std::string& option)
    {
        cc2Options_.push_back(option);
    }
    void SetDoPreprocess(bool v) { doPreprocess_ = v; }
    bool DoPreprocess() const { return doPreprocess_; }
    
    void SetSharedLibraryName(const std::string& name) 
    { 
        sharedLibraryName_ = name;
    }
    std::string SharedLibraryName() const { return sharedLibraryName_; }
    
    void SetMakeSharedLibrary(bool v) { makeSharedLibrary_ = v; }
    bool MakeSharedLibrary() const { return makeSharedLibrary_; }
    void SetPreprocessTwice(bool v) { preprocessTwice_ = v;}
    bool PreprocessTwice() const { return preprocessTwice_;}
    void SetMakeExecutable(bool v) { makeExecutable_ = v; }
    bool MakeExecutable() const { return makeExecutable_; }
    void SetRecognizeOccExtensions(bool v) { recognizeOccExtensions_ = v; }
    bool RecognizeOccExtensions() const { return recognizeOccExtensions_; }
    void RecordCmdOption(char*);
    void SetWcharSupport(bool v) { wcharSupport_ = v; }
    bool WcharSupport() const { return wcharSupport_; }
    
    void SetExternalDriver(bool v) { externalDriver_ = v; }
    bool ExternalDriver() const { return externalDriver_; }
    
    void SetStaticInitialization(bool v) { staticInitialization_ = v; }
    bool StaticInitialization() const { return staticInitialization_; }

    void SetDoCompile(bool v) { doCompile_ = v; }
    bool DoCompile() const { return doCompile_; }

    void SetDoTranslate(bool v) { doTranslate_ = v; }
    bool DoTranslate() const { return doTranslate_; }
    
    void SetSourceFileName(const std::string& s) { sourceFileName_ = s; }
    std::string SourceFileName() const { return sourceFileName_; }
    
    void SetOutputFileName(const std::string& s) { outputFileName_ = s; }
    std::string OutputFileName() const { return outputFileName_; }
    
    void SetCompilerCommand(const std::string& s) { compilerCommand_ = s; }
    std::string CompilerCommand() const { return compilerCommand_; }
    
    void SetLinkerCommand(const std::string& s) { linkerCommand_ = s; }
    std::string LinkerCommand() const { return linkerCommand_; }
    
    void SetPreprocessorCommand(const std::string& s) { preprocessorCommand_ = s; }
    std::string PreprocessorCommand() const { return preprocessorCommand_; }
    
    void SetPrintMetaclasses(bool v) { printMetaclasses_ = v; }
    bool PrintMetaclasses() const { return printMetaclasses_; }
    
    void AddObjectFile() { ++numOfObjectFiles_; }
    int NumOfObjectFiles() const { return numOfObjectFiles_; }
    
    void SetShowProgram(bool v) { showProgram_ = v; }
    bool ShowProgram() const { return showProgram_; }
    
    void SetShowVersion(bool v) { showVersion_ = v; }
    bool ShowVersion() const { return showVersion_; }
    
    void AddMetaclass(const std::string& metaclass) { metaclasses_.push_back(metaclass); }
    Iterator Metaclasses() const
    {
        return Iterator(
            std::auto_ptr<IteratorImpl>(
                new IteratorImpl(metaclasses_.begin(), metaclasses_.end())
            )
        );
    }
    
    Opencxx::ErrorLog& ErrorLog() const { return errorLog_; }

private:
    Opencxx::ErrorLog& errorLog_;
    bool verboseMode_;
    bool libtoolPlugins_;
    bool doPreprocess_;
    bool preprocessTwice_;
    bool makeExecutable_;
    bool makeSharedLibrary_;
    bool recognizeOccExtensions_;
    bool wcharSupport_;
    bool staticInitialization_;
    bool doCompile_;
    bool showProgram_;
    bool externalDriver_;
    bool doTranslate_;
    bool showVersion_;
    bool printMetaclasses_;
    int  numOfObjectFiles_;
    std::string sourceFileName_;
    std::string sharedLibraryName_;
    std::string outputFileName_;
    std::string compilerCommand_;
    std::string preprocessorCommand_;
    std::string linkerCommand_;
    
    std::vector<std::string> ccOptions_;
    std::vector<std::string> cc2Options_;
    std::vector<std::string> metaclasses_;
};

void ParseCommandLine(int argc, char** argv, OpencxxConfiguration& config);
    /* throws UnknownCliOptionException */

}

#endif /* ! guard_opencxx_OpencxxConfiguration_h */
