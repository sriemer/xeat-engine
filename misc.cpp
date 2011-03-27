#include "misc.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <pwd.h>

char permissionFlags(string str)
{
    char result = 0;
    if (str.find("r") != string::npos)
        result |= F_READ_ONLY;
    if (str.find("w") != string::npos)
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
