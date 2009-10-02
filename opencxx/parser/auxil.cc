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

#include <cstring>
#include <cassert>
#include <opencxx/parser/auxil.h>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

char* gc_aware_strdup(const char* src)
{
    assert(src);
    int len = strlen(src);
    char* result = new (GC) char[len+1];
    return strcpy(result, src);
}

char* IntegerToString(int num, int& length)
{
    const int N = 16;
    static char buf[N];
    bool minus;

    int i = N - 1;
    if(num >= 0)
        minus = false;
    else{
        minus = true;
        num = -num;
    }

    buf[i--] = '\0';
    if(num == 0){
        buf[i] = '0';
        length = 1;
        return &buf[i];
    }
    else{
        while(num > 0){
            buf[i--] = '0' + char(num % 10);
            num /= 10;
        }

        if(minus)
            buf[i--] = '-';

        length = N - 2 - i;
        return &buf[i + 1];
    }
}

}

