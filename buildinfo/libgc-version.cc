//@beginlicenses@
//@license{Grzegorz Jakacki}{2004}@
//@endlicenses@
#ifdef HAVE_CONFIG_H
#    include "config.h"
#endif

#include <cstring>
#include <cstdio>

#if DONT_GC
    unsigned GC_version = 0;
#else
    extern unsigned GC_version;
#endif

int main(int ac, const char* av[])
{
    if (ac >= 2  &&  strcmp(av[1],"--help")==0)
    {
        printf("%s", "Prints the version of libgc.\n");
    }
    if (GC_version)
    {
        printf("gc-%d.%d", 
            (GC_version & 0xffff0000) >> 16,
            (GC_version & 0x0000ff00) >> 8);
        unsigned alpha = GC_version & 0x000000ff;
        if (alpha != 0xff) printf("alpha%d", alpha);
        printf("\n");
    }
    else
    {
        printf("nogc");
    }
}
