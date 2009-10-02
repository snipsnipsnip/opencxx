#ifndef guard_opencxx_AbstractTypingWalker_h
#define guard_opencxx_AbstractTypingWalker_h

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

namespace Opencxx
{

class Ptree;
class TypeInfo;

class AbstractTypingWalker {
public:
    virtual void TypeofPtree(Ptree*, TypeInfo&) = 0; ///< catch all
    virtual void TypeofComma(Ptree*, TypeInfo&) = 0;
    virtual void TypeofAssign(Ptree*, TypeInfo&) = 0;
    virtual void TypeofCond(Ptree*, TypeInfo&) = 0;
    virtual void TypeofInfix(Ptree*, TypeInfo&) = 0;
    virtual void TypeofPm(Ptree*, TypeInfo&) = 0;
    virtual void TypeofCast(Ptree*, TypeInfo&) = 0;
    virtual void TypeofUnary(Ptree*, TypeInfo&) = 0;
    virtual void TypeofThrow(Ptree*, TypeInfo&) = 0;
    virtual void TypeofSizeof(Ptree*, TypeInfo&) = 0;
    virtual void TypeofTypeid(Ptree*, TypeInfo&) = 0;
    virtual void TypeofTypeof(Ptree*, TypeInfo&) = 0;
    virtual void TypeofNew(Ptree*, TypeInfo&) = 0;
    virtual void TypeofDelete(Ptree*, TypeInfo&) = 0;
    virtual void TypeofThis(Ptree*, TypeInfo&) = 0;
    virtual void TypeofVariable(Ptree*, TypeInfo&) = 0;
    virtual void TypeofFstyleCast(Ptree*, TypeInfo&) = 0;
    virtual void TypeofArray(Ptree*, TypeInfo&) = 0;
    virtual void TypeofFuncall(Ptree*, TypeInfo&) = 0;	// and fstyle cast
    virtual void TypeofPostfix(Ptree*, TypeInfo&) = 0;
    virtual void TypeofUserStatement(Ptree*, TypeInfo&) = 0;
    virtual void TypeofDotMember(Ptree*, TypeInfo&) = 0;
    virtual void TypeofArrowMember(Ptree*, TypeInfo&) = 0;
    virtual void TypeofParen(Ptree*, TypeInfo&) = 0;
    virtual void TypeofStaticUserStatement(Ptree*, TypeInfo&) = 0;
    
protected:
   virtual ~AbstractTypingWalker() {}
};

}

#endif /* ! guard_opencxx_AbstractTypingWalker_h */
