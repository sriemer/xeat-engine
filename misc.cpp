#include "misc.h"
#include <iostream>

char permissionFlags(string str)
{
    char result = 0;
    if (str.find("r") != string::npos)
        result |= F_READ_ONLY;
    if (str.find("w") != string::npos)
        result |= F_WRITE_ONLY;
    return result;
}
