#ifndef guard_opencxx_Class_h
#define guard_opencxx_Class_h

//@beginlicenses@
//@license{chiba-tokyo}{}@
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
   
#include <iosfwd>
#include <opencxx/ChangedMemberList.h>
#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/token-names.h>

namespace Opencxx
{

class ChangedMemberList;
class ClassArray;
class Environment;
class Member;
class MemberList;
class TypeInfo;

class Class : public Object {
public:
    Class() {}
    Class(Environment* e, char* name) { Construct(e, Ptree::Make(name)); }
    Class(Environment* e, Ptree* name) { Construct(e, name); }

    virtual void InitializeInstance(Ptree* def, Ptree* margs);
    virtual ~Class();

// introspection

    Ptree* Comments();
    Ptree* Name();
    Ptree* BaseClasses();
    Ptree* Members();
    Ptree* Definition() { return definition; }
    virtual char* MetaclassName();	// automaticallly implemented
					// by Metaclass
    Class* NthBaseClass(int nth);
    Ptree* NthBaseClassName(int nth);
    bool IsSubclassOf(Ptree* name);
    bool IsImmediateSubclassOf(Ptree* name);

    bool NthMember(int nth, Member& member);
    bool LookupMember(Ptree* name);
    bool LookupMember(Ptree* name, Member& member, int index = 0);
    bool LookupMember(char* name);
    bool LookupMember(char* name, Member& member, int index = 0);
    MemberList* GetMemberList();

    // These are available only within Finalize()
    static ClassArray& AllClasses();
    int Subclasses(ClassArray& subclasses);
    static int Subclasses(Ptree* name, ClassArray& subclasses);
    int ImmediateSubclasses(ClassArray& subclasses);
    static int ImmediateSubclasses(Ptree* name, ClassArray& subclasses);
    static int InstancesOf(char* metaclass_name, ClassArray& classes);

    // obsolete
    Ptree* NthMemberName(int);
    int IsMember(Ptree*);
    bool LookupMemberType(Ptree*, TypeInfo&);

// translation

    enum { Public = PUBLIC, Protected = PROTECTED, Private = PRIVATE, Undefined = 0 };

    virtual void TranslateClass(Environment*);
    void RemoveClass() { removed = true; }
    void AddClassSpecifier(Ptree* spec);	// only for MSVC++
    void ChangeName(Ptree* name);
    void ChangeBaseClasses(Ptree*);
    void RemoveBaseClasses();
    void AppendBaseClass(Class* c, int specifier = Public,
			 bool is_virtual = false);
    void AppendBaseClass(char* class_name, int specifier = Public,
			 bool is_virtual = false);
    void AppendBaseClass(Ptree* class_name, int specifier = Public,
			 bool is_virtual = false);

    void ChangeMember(Member& changed_member);
    void AppendMember(Member& added_member, int specifier = Public);
    void AppendMember(Ptree* added_member);
    void RemoveMember(Member&);

    virtual void TranslateMemberFunction(Environment*, Member&);

    virtual Ptree* TranslateInitializer(Environment*, Ptree* var_name,
					Ptree* initializer);

    virtual Ptree* TranslateNew(Environment*,
				Ptree* header, Ptree* new_operator,
				Ptree* placement, Ptree* type_name,
				Ptree* arglist);
    virtual Ptree* TranslateDelete(Environment*, Ptree* op, Ptree* obj);
    virtual Ptree* TranslateAssign(Environment*, Ptree* left,
				   Ptree* assign_op, Ptree* right);
    virtual Ptree* TranslateBinary(Environment*, Ptree* left,
				   Ptree* binary_op, Ptree* right);
    virtual Ptree* TranslateUnary(Environment*,
				  Ptree* unary_op, Ptree* object);
    virtual Ptree* TranslateSubscript(Environment*,
				      Ptree* object, Ptree* index);
    virtual Ptree* TranslatePostfix(Environment*,
				    Ptree* object, Ptree* postfix_op);
    virtual Ptree* TranslateFunctionCall(Environment*,
					 Ptree* object, Ptree* arglist);
    virtual Ptree* TranslateMemberCall(Environment*,
				       Ptree* object, Ptree* access_op,
				       Ptree* member_name,
				       Ptree* arglist);
    virtual Ptree* TranslateMemberCall(Environment*,
				       Ptree* member_name, Ptree* arglist);
    virtual Ptree* TranslateMemberRead(Environment*,
				       Ptree* object, Ptree* access_op,
				       Ptree* member_name);
    virtual Ptree* TranslateMemberRead(Environment*, Ptree* member_name);
    virtual Ptree* TranslateMemberWrite(Environment*,
					Ptree* object, Ptree* access_op,
					Ptree* member_name,
					Ptree* assign_op, Ptree* expr);
    virtual Ptree* TranslateMemberWrite(Environment*,
					Ptree* member_name,
					Ptree* assign_op, Ptree* expr);
    virtual Ptree* TranslateUnaryOnMember(Environment*, Ptree* unary_op,
					  Ptree* object, Ptree* access_op,
					  Ptree* member_name);
    virtual Ptree* TranslateUnaryOnMember(Environment*, Ptree* unary_op,
					  Ptree* member_name);
    virtual Ptree* TranslatePostfixOnMember(Environment*,
					    Ptree* object, Ptree* access_op,
					    Ptree* member_name,
					    Ptree* postfix_op);
    virtual Ptree* TranslatePostfixOnMember(Environment*,
					    Ptree* member_name,
					    Ptree* postfix_op);
    virtual Ptree* TranslatePointer(Environment*, Ptree* var_name);

    virtual Ptree* TranslateUserStatement(Environment*,
					  Ptree* object, Ptree* access_op,
					  Ptree* keyword, Ptree* rest);
    virtual Ptree* TranslateStaticUserStatement(Environment*,
						Ptree* keyword, Ptree* rest);

    static Ptree* StripClassQualifier(Ptree* qualified_name);

    Ptree* TranslateExpression(Environment*, Ptree* expr);
    Ptree* TranslateExpression(Environment*, Ptree* expr, TypeInfo& type);
    Ptree* TranslateNewType(Environment* env, Ptree* type);
    Ptree* TranslateArguments(Environment*, Ptree* arglist);
    Ptree* TranslateFunctionBody(Environment*, Member& m, Ptree* body);

// others

    Environment* GetEnvironment() { return class_environment; }
    virtual bool AcceptTemplate();
    static bool Initialize();
    static void FinalizeAll(std::ostream& out);
    virtual Ptree* FinalizeInstance();
    virtual Ptree* Finalize();		// obsolete
    static Ptree* FinalizeClass();

    static void RegisterNewModifier(char* keyword);
    static void RegisterNewAccessSpecifier(char* keyword);
    static void RegisterNewMemberModifier(char* keyword);
    static void RegisterNewWhileStatement(char* keyword);
    static void RegisterNewForStatement(char* keyword);
    static void RegisterNewClosureStatement(char* keyword);
    static void RegisterMetaclass(char* keyword, char* class_name);

    static void ChangeDefaultMetaclass(char* name);
    static void ChangeDefaultTemplateMetaclass(char* name);
    static void SetMetaclassForFunctions(char* name);

    static void InsertBeforeStatement(Environment*, Ptree*);
    static void AppendAfterStatement(Environment*, Ptree*);
    static void InsertBeforeToplevel(Environment*, Class*);
    static void InsertBeforeToplevel(Environment*, Member&);
    static void InsertBeforeToplevel(Environment*, Ptree*);
    static void AppendAfterToplevel(Environment*, Class*);
    static void AppendAfterToplevel(Environment*, Member&);
    static void AppendAfterToplevel(Environment*, Ptree*);
    bool InsertDeclaration(Environment*, Ptree* declaration);
    bool InsertDeclaration(Environment*, Ptree* declaration,
			   Ptree* key, void* client_data);
    void* LookupClientData(Environment*, Ptree* key);

    void ErrorMessage(Environment*, char* message, Ptree* name,
		      Ptree* where);
    void WarningMessage(Environment*, char* message, Ptree* name,
			Ptree* where);
    void ErrorMessage(char* message, Ptree* name, Ptree* where);
    void WarningMessage(char* message, Ptree* name, Ptree* where);

    static bool RecordCmdLineOption(char* key, char* value);
    static bool LookupCmdLineOption(char* key);
    static bool LookupCmdLineOption(char* key, char*& value);

private:
    void Construct(Environment*, Ptree*);

    void SetEnvironment(Environment*);
    Ptree* GetClassSpecifier() { return new_class_specifier; }
    Ptree* GetNewName() { return new_class_name; }
    Ptree* GetBaseClasses() { return new_base_classes; }
    ChangedMemberList::Cmem* GetChangedMember(Ptree*);
    ChangedMemberList* GetAppendedMembers() { return appended_member_list; }
    Ptree* GetAppendedCode() { return appended_code; }
    void TranslateClassHasFinished() { done_decl_translation = true; }
    void CheckValidity(char*);

private:
    Ptree* definition;
    Ptree* full_definition;	// including a user keyword
    Environment* class_environment;
    MemberList* member_list;

    bool done_decl_translation;
    bool removed;
    ChangedMemberList* changed_member_list;
    ChangedMemberList* appended_member_list;
    Ptree* appended_code;
    Ptree* new_base_classes;
    Ptree* new_class_specifier;
    Ptree* new_class_name;

    static ClassArray* class_list;

    enum { MaxOptions = 8 };
    static int num_of_cmd_options;
    static char* cmd_options[MaxOptions * 2];

    static char* metaclass_for_c_functions;
    static Class* for_c_functions;

    static Ptree* class_t;
    static Ptree* empty_block_t;
    static Ptree* public_t;
    static Ptree* protected_t;
    static Ptree* private_t;
    static Ptree* virtual_t;
    static Ptree* colon_t;
    static Ptree* comma_t;
    static Ptree* semicolon_t;

friend class Walker;
friend class ClassWalker;
friend class ClassBodyWalker;
friend class Member;
};

}

#endif /* ! guard_opencxx_Class_h */
