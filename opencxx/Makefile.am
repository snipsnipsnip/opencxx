## Makefile.am -- Process this file with automake to produce Makefile.in
#-------------------------------------------------------------------------
# Permission to use, copy, distribute and modify this software and its
# documentation for any purpose is hereby granted without fee, provided that
# the above copyright notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting documentation.
#
# Other Contributors make no representations about the suitability of this
# software for any purpose. It is provided "as is" without express or implied
# warranty.
#
# 2001-2003 (C) Copyright by Other Contributors.
#-------------------------------------------------------------------------
AUTOMAKE_OPTIONS = subdir-objects

abs_top_builddir = @abs_top_builddir@
abs_builddir = @abs_builddir@
srcdir = @srcdir@

BUILT_SOURCES = parser/ptree-generated.h parser/ptree-generated.cc

nobase_pkginclude_HEADERS = \
	parser/AbstractTranslatingWalker.h \
	parser/AbstractTypingWalker.h \
	parser/auxil.h \
	parser/BigHashTable.h \
	parser/CerrErrorLog.h \
	parser/deprecated.h \
	parser/DupLeaf.h \
	parser/Encoding.h \
	parser/ErrorLog.h \
	parser/GC-disabled.h \
	parser/GC-enabled.h \
	parser/GC.h \
	parser/HashTable.h \
	parser/Leaf.h \
	parser/LeafName.h \
	parser/LeafReserved.h \
	parser/LeafThis.h \
	parser/Lex.h \
	parser/MetaclassLoader.h \
	parser/MopMsg.h \
	parser/Msg.h \
	parser/NonLeaf.h \
	parser/NullMetaclassLoader.h \
	parser/ParseErrorMsg.h \
	parser/Parser.h \
	parser/Program.h \
	parser/ProgramFile.h \
	parser/ProgramFromStdin.h \
	parser/ProgramString.h \
	parser/PtreeAccessDecl.h \
	parser/PtreeAccessSpec.h \
	parser/PtreeAndEncodingPrinter.h \
	parser/PtreeArray.h \
	parser/PtreeBlock.h \
	parser/PtreeBrace.h \
	parser/PtreeClassBody.h \
	parser/PtreeClassSpec.h \
	parser/PtreeConstants.h	\
	parser/PtreeDeclaration.h \
	parser/PtreeDeclarator.h \
	parser/PtreeEnumSpec.h \
	parser/PtreeExprStatement.h \
	parser/PtreeExternTemplate.h \
	parser/PtreeFstyleCastExpr.h \
	parser/ptree-generated.h \
	parser/ptreeAll.h \
	parser/Ptree.h \
	parser/PtreeLinkageSpec.h \
	parser/PtreeMetaclassDecl.h \
	parser/PtreeName.h \
	parser/PtreeNamespaceAlias.h \
	parser/PtreeNamespaceSpec.h \
	parser/PtreeTemplateDecl.h \
	parser/PtreeTemplateInstantiation.h \
	parser/PtreeTypedef.h \
	parser/PtreeUserAccessSpec.h \
	parser/PtreeUserdefKeyword.h \
	parser/PtreeUsing.h \
	parser/PtreeUtil.h \
	parser/SourceLocation.h \
	parser/TheErrorLog.h \
	parser/Token.h \
	parser/token-names.h \
	Bind.h					\
	BindClassName.h				\
	BindEnumName.h				\
	BindTemplateClass.h			\
	BindTemplateFunction.h			\
	BindTypedefName.h			\
	BindVarName.h				\
	cbodywalk.h				\
	classwalk.h				\
	ChangedMemberList.h			\
	Class.h					\
	ClassArray.h				\
	ClassBodyWalker.h			\
	ClassWalker.h 				\
	CliErrorMsg.h			\
	defs.h					\
	dloading.h			\
	driver.h 				\
	driver2.h			\
	DynamicMopMetaclassLoader.h	\
	EncodingUtil.h			\
	Environment.h				\
	GenericMsg.h				\
	InfoMsg.h			\
	memberAll.h				\
	Member.h				\
	MemberFunction.h			\
	MemberList.h				\
	Metaclass.h 				\
	MetaclassRegistration.h 		\
	MetaclassRegistry.h			\
	MetacompilerConfiguration.h	\
	mop.h					\
	mop2.h					\
	nil.h				\
	OpencxxConfiguration.h		\
	PtreeIter.h				\
	PtreeMaker.h				\
	PtreeTypeUtil.h			\
	QuoteClass.h				\
	TemplateClass.h				\
	TheMetaclassRegistry.h			\
	TypeInfo.h 				\
	UnknownCliOptionException.h	\
	Walker.h


EXTRA_DIST	= \
		parser/ptree-gen.in \
		parser/GC.h.in

INCLUDES = -I$(builddir) -I$(top_srcdir) $(INCLTDL) 

noinst_LTLIBRARIES		= \
	libocc_mop.la 	\
	libocc_parser.la

libocc_mop_la_LIBADD 	= libocc_parser.la

libocc_mop_la_SOURCES	= 			\
	Bind.cc					\
	BindClassName.cc			\
	BindEnumName.cc				\
	BindTemplateClass.cc			\
	BindTemplateFunction.cc			\
	BindTypedefName.cc			\
	BindVarName.cc				\
	ChangedMemberList.cc			\
	Class.cc				\
	ClassArray.cc				\
	ClassBodyWalker.cc			\
	ClassWalker.cc 				\
	dloading.cc				\
	driver.cc 				\
	driver2.cc 				\
	EncodingUtil.cc			\
	Environment.cc				\
	main-con.cc			\
	Member.cc				\
	MemberFunction.cc			\
	MemberList.cc				\
	Metaclass.cc 				\
	MetaclassRegistry.cc			\
	OpencxxConfiguration.cc			\
	PtreeIter.cc				\
        PtreeMaker.cc				\
	PtreeTypeUtil.cc			\
	QuoteClass.cc				\
	TemplateClass.cc			\
	TheMetaclassRegistry.cc			\
	TypeInfo.cc 				\
	Walker.cc

libocc_mop_la_DEPENDENCIES = Makefile.am libocc_parser.la

libocc_parser_la_SOURCES	= \
	parser/auxil.cc \
	parser/BigHashTable.cc \
	parser/CerrErrorLog.cc \
	parser/deprecated.cc \
	parser/DupLeaf.cc \
	parser/Encoding.cc \
	parser/HashTable.cc \
	parser/Leaf.cc \
	parser/LeafName.cc \
	parser/LeafThis.cc \
	parser/Lex.cc \
	parser/NonLeaf.cc \
	parser/Parser.cc \
	parser/Program.cc \
	parser/ProgramFile.cc \
	parser/ProgramFromStdin.cc \
	parser/ProgramString.cc \
	parser/PtreeAccessDecl.cc \
	parser/PtreeAccessSpec.cc \
	parser/PtreeArray.cc \
	parser/PtreeBlock.cc \
	parser/PtreeBrace.cc \
	parser/Ptree.cc \
	parser/PtreeClassBody.cc \
	parser/PtreeClassSpec.cc \
	parser/PtreeDeclaration.cc \
	parser/PtreeDeclarator.cc \
	parser/PtreeEnumSpec.cc \
	parser/PtreeExprStatement.cc \
	parser/PtreeExternTemplate.cc \
	parser/PtreeFstyleCastExpr.cc \
	parser/PtreeConstants.cc \
	parser/ptree-generated.cc \
	parser/PtreeLinkageSpec.cc \
	parser/PtreeMetaclassDecl.cc \
	parser/PtreeName.cc \
	parser/PtreeNamespaceAlias.cc \
	parser/PtreeNamespaceSpec.cc \
	parser/PtreeTemplateDecl.cc \
	parser/PtreeTemplateInstantiation.cc \
	parser/PtreeTypedef.cc \
	parser/PtreeUserAccessSpec.cc \
	parser/PtreeUserdefKeyword.cc \
	parser/PtreeUsing.cc \
	parser/PtreeUtil.cc \
	parser/TheErrorLog.cc

libocc_parser_la_DEPENDENCIES = Makefile.am

###################################################################

parser/ptree-gen: $(srcdir)/parser/ptree-gen.in
	cd $(top_builddir) && ./config.status opencxx/parser/ptree-gen

parser/ptree-generated.cc: parser/ptree-generated.h parser/ptree-gen
parser/ptree-generated.h: parser/ptree-gen
	cd parser && chmod +x ptree-gen && ./ptree-gen

check-header-completness: $(nobase_pkginclude_HEADERS:%=%.is-complete) $(noinst_HEADERS:%=%.is-complete)

%.h.is-complete : %.h
	rm -f completness.cc
	echo '#include "'$<'"' >completness.cc
	$(CXX) -c $(INCLUDES) completness.cc -o completness.o
	rm completness.cc completness.o
	touch $@

show-sources:
	@for f in $(nobase_pkginclude_HEADERS) \
                  $(EXTRA_DIST) $(libocc_mop_la_SOURCES) \
                  $(libocc_parser_la_SOURCES); do echo $$f; done
