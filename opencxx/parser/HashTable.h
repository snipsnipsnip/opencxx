#ifndef guard_opencxx_HashTable_h
#define guard_opencxx_HashTable_h

//@beginlicenses@
//@license{chiba_tokyo}{}@
//@license{xerox}{}@
//@license{contributors}{}@
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  1997-2001 Shigeru Chiba, Tokyo Institute of Technology. make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
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

#include <iosfwd>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

class HashTable : public LightObject {
public:
  typedef void *Value;

  struct Entry
  {
    char *key;		// 0: unused, -1: deleted
    Value value;
  };


    HashTable();
    HashTable(int) {}
    void MakeTable();
    bool IsEmpty();
    void Dump(std::ostream&);
    int AddEntry(char* key, Value value, int* index = 0);
    int AddEntry(bool, char* key, int len, Value value, int* index = 0);

    int AddEntry(char* key, int len, Value value, int* index = 0) {
	return AddEntry(true, key, len, value, index);
    }

    // allow a duplicated entry to be inserted
    int AddDupEntry(char* key, int len, Value value, int* index = 0) {
	return AddEntry(false, key, len, value, index);
    }

    bool Lookup(char* key, Value *value);
    bool Lookup(char* key, int len, Value *value);
    bool LookupEntries(char* key, int len, Value *value, int& nth);
    Value Peek(int index);
    bool RemoveEntry(char* key);
    bool RemoveEntry(char* key, int len);
    void ReplaceValue(int index, Value value);

protected:
    char* KeyString(char* key);
    char* KeyString(char* key, int len);

    bool Lookup2(char* key, Value *val, int* index);
    bool Lookup2(char* key, int len, Value *val, int* index);
    static unsigned NextPrimeNumber(unsigned number);
    bool GrowTable(int increment);
    unsigned int StringToInt(char*);
    unsigned int StringToInt(char*, int);
    int HashFunc(unsigned int p, int n);

protected:
    Entry *entries;
    int	   Size;        // the max number of entries.
              	        // should be a prime number
    int	   Prime2;
};

}

#endif /* ! guard_opencxx_HashTable_h */
