/* dummy implementation of strtol */
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

long int strtol(const char* nPtr, char** endPtr, int base)
{
    const char *s = nPtr;
    bool isNegative = false;
    unsigned long cutoff;
    int any = 0;
    unsigned long acc = 0;
    int cutlim;
    if (endPtr) {
        *endPtr = (char*)nPtr;
    }

    if (base != 0 && (base < 2 || base > 36))
    {
        errno = EINVAL;
        return 0;
    }

    while(isspace((unsigned char)*s))
    {
        s++;
    }

    if (*s == '-')
    {
        isNegative = true;
        s++;
    } else if (*s == '+') {
        s++;
    }

    if (base == 0)
    {
        if (*s == '0')
        {
            if ((s[1] == 'x' || s[1] == 'X') && isxdigit((unsigned char)s[2])) 
            {
                base = 16;
                s+=2;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    } else if (base ==  16) {
        if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X') &&
            isxdigit((unsigned char)s[2])) {
            s+=2;
        }
    }

    cutoff = isNegative ? -(unsigned long)LONG_MIN : LONG_MAX;
    cutlim = (int)(cutoff % (unsigned long)base);
    cutoff /= (unsigned long)base; 

    for(; *s; s++)
    {
        int c = (unsigned char)*s;
        int digit;

        if (c >= '0' && c <= '9')
        {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 10;
        } else {
            break;
        }

        if (digit >= base){
            break;
        }

        if (any < 0) {
            continue;
        }

        if (acc > cutoff || (acc == cutoff && digit > cutlim)){
            any = -1;
            acc = cutoff;
            errno = ERANGE;
        } else {
            any = 1;
            acc = acc * (unsigned long)base + (unsigned long)digit;
        }
    }

    if (any == 0)
    {
        if (endPtr)
        {
            *endPtr = (char *)nPtr;
        }
        errno = EINVAL;
        return 0;
    }

    if (endPtr)
    {
        *endPtr = (char *)s;
    }

    if (any < 0) 
    {
        return isNegative ? LONG_MIN : LONG_MAX;
    }

    if (isNegative){
        return -(long)acc;
    }

    return (long)acc;
}
