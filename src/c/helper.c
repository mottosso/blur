#include <stdio.h>
#include <ctype.h>

#include "helpers.h"

/* We can't use strings.h, due to compatibility with MSVC */
int strcasecmp(const char* s1, const char* s2)
{
    for (;;) {
        int c1 = tolower( *((unsigned char*) s1++));
        int c2 = tolower( *((unsigned char*) s2++));

        if ((c1 != c2) || (c1 == '\0')) {
            return( c1 - c2);
        }
    }
}