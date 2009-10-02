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

#include <iostream>
#include <cstring>
#include <opencxx/parser/HashTable.h>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

using std::cerr;

HashTable::HashTable()
{
    Size = 251;
    Prime2 = 127;
    MakeTable();
}

void HashTable::MakeTable()
{
    entries = new (GC) Entry[Size];
    for(int i = 0; i < Size; ++i)
	entries[i].key = 0;
}

bool HashTable::IsEmpty()
{
    for(int i = 0; i < Size; ++i)
	if(entries[i].key != 0 && entries[i].key != (char*)-1)
	    return false;

    return true;
}

void HashTable::Dump(std::ostream& out)
{
    out << '{';
    for(int i = 0; i < Size; ++i)
	if(entries[i].key != 0 && entries[i].key != (char*)-1)
//	    out << entries[i].key << ", ";
	    out << entries[i].key << '(' << i << "), ";

    out << '}';
}

char* HashTable::KeyString(char* key) {
    char* str = new (GC) char[strlen(key) + 1];
    strcpy(str, key);
    return str;
}

char* HashTable::KeyString(char* key, int len) {
    char* str = new (GC) char[len + 1];
    memmove(str, key, len);
    str[len] = '\0';
    return str;
}

bool HashTable::Lookup(char* key, Value *value)
{
    int i;
    return Lookup2(key, value, &i);
}

bool HashTable::Lookup(char* key, int len, Value *value)
{
    int i;
    return Lookup2(key, len, value, &i);
}

bool HashTable::Lookup2(char* key, Value *value, int* index)
{
    unsigned int p = StringToInt(key);
    for(int i = 0; i < Size; ++i){
	int j = HashFunc(p, i);
	if(entries[j].key == 0){
	    return false;		// not found
	}
	else if(entries[j].key != (char*)-1
				&& strcmp(entries[j].key, key) == 0){
	    *value = entries[j].value;
	    *index = j;
	    return true;
	}
    }

    return false;
}

bool HashTable::Lookup2(char* key, int len, Value *value, int* index)
{
    unsigned int p = StringToInt(key, len);
    for(int i = 0; i < Size; ++i){
	int j = HashFunc(p, i);
	if(entries[j].key == 0){
	    return false;		// not found
	}
	else if(entries[j].key != (char*)-1
		&& strncmp(entries[j].key, key, len) == 0
		&& entries[j].key[len] == '\0'){
	    *value = entries[j].value;
	    *index = j;
	    return true;
	}
    }

    return false;
}

/*
  LookupEntries() is used to find multiple entries recorded with the
  same key.  It returns the entry found with the nth (>= 0) hash key.
  After this function completes, nth is increamented for the next try.
  The next entry can be found if nth is passed to LookupEntries() as is.
*/
bool HashTable::LookupEntries(char* key, int len, Value *value,
			      int& nth)
{
    unsigned int p = StringToInt(key, len);
    for(int i = nth; i < Size; ++i){
	int j = HashFunc(p, i);
	if(entries[j].key == 0){
	    return false;		// not found
	}
	else if(entries[j].key != (char*)-1
		&& strncmp(entries[j].key, key, len) == 0
		&& entries[j].key[len] == '\0'){
	    *value = entries[j].value;
	    nth = i + 1;
	    return true;
	}
    }

    return false;
}

/*
  A naive implementation to calculate a prime number.
  This function returns the first prime number being greater
  than or equal to 'number'.
*/
unsigned HashTable::NextPrimeNumber(unsigned number)
{
    if(number < 2)
	return 2;

    for(;;){
        unsigned half = number / 2;
        bool prime = true;
        for(unsigned i = 2; i <= half && prime; ++i)
            if(number % i == 0)
		prime = false;

        if(prime)
	    return number;

        ++number;
    }
}

/*
  WARNING! When an hashtable is expanded, the elements change of position!
  This means that the index returned by some HashTable methods is safely valid
  until the next insertion of a new element. So don't store such an index for
  a long period!

  Post condition : new Size >= old Size + 2 * increment
*/
bool HashTable::GrowTable(int increment)
{
    HashTable bigger(0);

    bigger.Prime2 = (int)NextPrimeNumber(Prime2 + increment);
    bigger.Size = (int)NextPrimeNumber(2 * bigger.Prime2);
    bigger.MakeTable();
    
    bool done = true;
    for(int i = 0; done && i < Size; ++i) {
        char *key = this->entries[i].key;
        if (key != 0 && key != (char*)-1)
	    done = bool(bigger.AddDupEntry(key, strlen(key), entries[i].value)
			>= 0);
    }

    if(done){
        entries = bigger.entries;
        Size = bigger.Size;
        Prime2 = bigger.Prime2;
    }

    return done;
}

// AddEntry adds a new entry to the hash table.
// If succeeding, this returns an index of the added entry, otherwise -1.
// Because `key' is duplicated, you can delete `key' later on.

int HashTable::AddEntry(char* key, Value value, int* index)
{
    unsigned int p = StringToInt(key);
    for(int i = 0; i < Size; ++i){
	int j = HashFunc(p, i);
	if(entries[j].key == 0 || entries[j].key == (char*)-1){
	    entries[j].key = KeyString(key);
	    entries[j].value = value;
	    if(index != 0)
		*index = j;

	    return j;
	}
	else if(strcmp(entries[j].key, key) == 0){
	    if(index != 0)
		*index = j;

	    return -1;		// it is already registered.
	}
    }

    if(GrowTable(1000))
	return AddEntry(key, value, index);

    cerr << "HashTable overflow (key: " << key << ")\nPanic...\n";
    if(index != 0)
	*index = 0;		// no meaning

    return -1;
}

int HashTable::AddEntry(bool check_duplication,
			char* key, int len, Value value, int* index)
{
    int i;
    unsigned int p = StringToInt(key, len);
    for(i = 0; i < Size; ++i){
	int j = HashFunc(p, i);
	if(entries[j].key == 0 || entries[j].key == (char*)-1){
	    entries[j].key = KeyString(key, len);
	    entries[j].value = value;
	    if(index != 0)
		*index = j;

	    return j;
	}
	else if(check_duplication
		&& strncmp(entries[j].key, key, len) == 0
		&& entries[j].key[len] == '\0'){
	    if(index != 0)
		*index = j;

	    return -1;		// it is already registered.
	}
    }

    if(GrowTable(1000))
	return AddEntry(check_duplication, key, len, value, index);

    cerr << "HashTable overflow (key: ";
    for(i = 0; i < len; ++i)
	cerr << key[i];

    cerr << ")\nPanic...\n";
    if(index != 0)
	*index = 0;		// no meaning

    return -1;
}

HashTable::Value HashTable::Peek(int index)
{
    return entries[index].value;
}

void HashTable::ReplaceValue(int index, Value val)
{
    if(0 <= index && index < Size)
	entries[index].value = val;
    else
	cerr << "HashTable: invalid index (" << index << ")\n";
}

bool HashTable::RemoveEntry(char* key)
{
    Value	u;
    int		index;

    if(!Lookup2(key, &u, &index))
	return false;		// not found
    else{
	entries[index].key = (char*)-1;
	return true;
    }
}

bool HashTable::RemoveEntry(char* key, int len)
{
    Value	u;
    int		index;

    if(!Lookup2(key, len, &u, &index))
	return false;		// not found
    else{
	entries[index].key = (char*)-1;
	return true;
    }
}

unsigned int HashTable::StringToInt(char* key)
{
    if(key == 0)
	return 0;

    unsigned int p = 0;
    unsigned int i, j;
    for(i = j = 0; key[i] != '\0'; ++i, ++j){
	if(j >= sizeof(unsigned int) * 8 - 7)
	    j = 0;

	p += key[i] << j;
    }

    return p;
}

unsigned int HashTable::StringToInt(char* key, int len)
{
    if(key == 0)
	return 0;

    unsigned int p = 0;
    int i;
    unsigned int j;
    for(i = j = 0; i < len; ++i, ++j){
	if(j >= sizeof(unsigned int) * 8 - 7)
	    j = 0;

	p += key[i] << j;
    }

    return p;
}

int HashTable::HashFunc(unsigned int p, int n)
{
    return((p + (unsigned int)n * (p % Prime2 + 1)) % Size);
}

}
