#include "misc.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <pwd.h>

// alias for string.h functions
// so they are not so cryptic

#define StringCopy(target,source) strcpy(target,source)
#define StringFind(target,character) strchr(target,character)
#define StringToInt(target) atoi(target)
#define StringToLong(target) atoll(target)
#define StringToDouble(target) atof(target)

#ifndef NULL
    #define NULL 0
#endif

char permissionFlags(char * str)
{
    char result = 0;
    if (StringFind(str,'r') != NULL)
        result |= F_READ_ONLY;
    if (StringFind(str,'w') != NULL)
        result |= F_WRITE_ONLY;
    return result;
}

const char * homeDirectory(pid_t target)
{
    // Get uid of the running program
    int uid;
    char tmp[30];
    sprintf(tmp,"/proc/%i/status",target);
    std::ifstream status(tmp);
    status >> tmp;
    while(strcmp(tmp,"Uid:"))
        status >> tmp;
    status >> uid;
    struct passwd *pw = getpwuid(uid);
    return pw->pw_dir;
}
