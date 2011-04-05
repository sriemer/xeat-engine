#ifndef MEMORY_H
#define MEMORY_H

// This file includes all needed to read from another process memory

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "misc.h"
#include "reader.h"

using namespace std;

enum SCAN_TYPE
{
    EXACT, BIGGER, SMALLER, BETWEEN, INCREASED, DECREASED, INCREASEDBY, DECREASEDBY, CHANGED, UNCHANGED, FIRST, UNKOWN
};

/* to use with /proc/pid/maps */
struct s_map
{

    void * start;
    void * end;
    char perms[5];
    void * offset;
    // ignored device
    // ignored inode
    // ignored pathname
};


// This buffer size is how much memory the program will read at once while doing an snapshot.
#define BUFFER_SIZE 512

bool attach(pid_t target);

bool detach(pid_t target);

/* read region using /proc/pid/mem */
ssize_t readregion(pid_t target, void *buf, size_t count, unsigned long offset);

/* write region using ptrace */
ssize_t writeregion(pid_t target, void *buf, size_t count, unsigned long offset);

/* Returns readable regions */
vector<s_map> readableRegions(pid_t target);

/* Returns writeable (and readable) regions */
vector<s_map> writeableRegions(pid_t target);

/* Returns the next readable block of size of the target */
void * nextRegion(pid_t target, void * lastRegion, size_t size);

/* Creates an snapshot in a temporary file and returns a pointer to a FILE that manages it */
FILE * snapshot(pid_t target, bool writeable = false);

/* Uses the lastScan FILE for continue scans */
FILE * searchValue(pid_t target,FILE * lastScan, bool readonly, class any_value& buff, SCAN_TYPE stype, long * ocurrences);


#endif // MEMORY_H
