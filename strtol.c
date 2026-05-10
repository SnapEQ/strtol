/* dummy implementation of strtol */
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


const char *skip_whitespace(const char *s)
{
    while (isspace((unsigned char)*s))
    {
        s++;
    }
    return s;
}

const char *parse_sign(const char *s, bool *isNegative)
{
    if (*s == '-')
    {
        *isNegative = true;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }
    return s;
}

const char *determine_base(const char *s, int *base)
{
    if (*base == 0)
    {
        if (*s == '0')
        {
            if ((s[1] == 'x' || s[1] == 'X') && isxdigit((unsigned char)s[2]))
            {
                *base = 16;
                return s + 2;
            }
            else
            {
                *base = 8;
            }
        }
        else
        {
            *base = 10;
        }
    }
    else if (*base == 16)
    {
        if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X') &&
            isxdigit((unsigned char)s[2]))
        {
            return s + 2;
        }
    }
    return s;
}

int char_to_digit(char c, int base)
{
    int digit = -1;

    if (c >= '0' && c <= '9')
        digit = c - '0';
    else if (c >= 'A' && c <= 'Z')
        digit = c - 'A' + 10;
    else if (c >= 'a' && c <= 'z')
        digit = c - 'a' + 10;

    return (digit >= base) ? -1 : digit;
}


void setup_limits(bool isNegative, int base,
                         unsigned long *cutoff, int *cutlim)
{
    *cutoff = isNegative ? -(unsigned long)LONG_MIN : LONG_MAX;
    *cutlim = (int)(*cutoff % (unsigned long)base);
    *cutoff /= (unsigned long)base;
}

const char *parse_digits(const char *s, int base, unsigned long cutoff,
                                int cutlim, unsigned long *acc, int *any)
{
    while (*s)
    {
        int digit = char_to_digit(*s, base);
        if (digit == -1)
            break;

        if (*any < 0)
        {
            s++;
            continue;
        }

        if (*acc > cutoff || (*acc == cutoff && digit > cutlim))
        {
            *any = -1;
            *acc = cutoff;
            errno = ERANGE;
        }
        else
        {
            *any = 1;
            *acc = *acc * (unsigned long)base + (unsigned long)digit;
        }
        s++;
    }
    return s;
}

long int strtol(const char *nPtr, char **endPtr, int base)
{
    const char *s = nPtr;
    bool isNegative = false;
    unsigned long cutoff;
    int cutlim;
    int any = 0;
    unsigned long acc = 0;

    if (endPtr)
    {
        *endPtr = (char *)nPtr;
    }

    if (base != 0 && (base < 2 || base > 36))
    {
        errno = EINVAL;
        return 0;
    }

    s = skip_whitespace(s);
    s = parse_sign(s, &isNegative);
    s = determine_base(s, &base);
    setup_limits(isNegative, base, &cutoff, &cutlim);
    s = parse_digits(s, base, cutoff, cutlim, &acc, &any);

    if (any == 0)
    {
        if (endPtr)
        {
            *endPtr = (char *)nPtr;
        }
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

    if (isNegative)
        return -(long)acc;

    return (long)acc;
}
