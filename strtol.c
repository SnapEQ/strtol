/* dummy implementation of strtol */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

char *getValidBases(const char* bases, int base);
bool isValidChar(char c, const char* validBases, int base);


long int strtol(const char* nPtr, char** endPtr, int base)
{
    if (endPtr) {
        *endPtr = (char*)nPtr;
    }

    if ((base < 2 || base > 36) && base != 0)
    {
        return 0;
    }

    bool isAutoDetect = false;

    if (base == 0)
    {
        isAutoDetect = true;
    }

    bool isOctal = false;
    bool isHex = false;

    if (strlen(nPtr) > 2 || isAutoDetect)
    {
        if (nPtr[0] == '0')
        {
            if (nPtr[1] == 'x' || nPtr[1] == 'X'){
                isHex = true;
                base = 16;

            } else {
                isOctal = true;
                base = 8;
            }

        }
    }

    const char bases[]="0123456789ABDCEFGHIJKLMNOPQRSTUVWXYZ";

    char *validBases = getValidBases(bases, base);

    if (validBases == NULL){
        return 0;
    }
 

    free(validBases);
    return 0;
}

char *getValidBases(const char* bases, int base)
{
    char *result = malloc(base * sizeof(char));

    if(result == NULL){
        fprintf(stderr, "Failure to allocate memory");
        return NULL;
    }

    for (int i = 0; i < base; i++)
    {
        result[i] = bases[i];
    }

    return result;
}

bool isValidChar(const char c, const char* validBases, int base)
{
    for(int i = 0; i < base; i++)
    {
        if(c == validBases[i])
        {
            return true;
        }
    }

    return false;
}
