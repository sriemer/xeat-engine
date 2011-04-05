#ifndef MISC_H
#define MISC_H

#include <string>

using std::string;

// flags
#define F_READ_ONLY 0x01
#define F_WRITE_ONLY 0x02
#define F_READ_WRITE 0x01 | 0x02

// This function must get something like
// rwxp, r--p, ---p, rw-p, r-xp...
char permissionFlags(char * str);

// Returns ~/ of a determinated program.
const char * homeDirectory(pid_t target);

// Alias:

#ifdef _DEBUG
#define show_info printf
#endif
#define show_error printf

#endif // MISC_H

