#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <fstream>
#include <string>
#include <sstream>
#include <pwd.h>
#include "misc.h"


using std::ofstream;
using std::string;

static timeval * timezero = 0;
static timeval lasttv;
static timeval offsettv;
static ofstream * outputMemory = 0;

// Output memory is a file to show where is int M and int N in memory,
// so you can modify speed hack in run time without the need of recompile library:
typedef int (*go)(timeval *tv, class timezone *tz);

static go gettimeofday_orig = 0;

// Multiply speed:
int M = 1;
// Divide speed:
int N = 1;
// That means 1/2 speed;

int M_old = M;
int N_old = N;

// Output memory is a file to show where is int M and int N in memory,
// so you can modify speed hack in run time without the need of recompile library:
void outputMandN()
{
    // Get home directory
    struct passwd *pw = getpwuid(getuid());

    string s = pw->pw_dir;

    // Adding some information to directory:

    // Makedir if not exists:
    // mkdir -p "~/.speedhack/"
    std::stringstream buf;
    buf << "mkdir -p \"" << homeDirectory(getpid()) << "/.speedhack/\"";
    system(buf.str().c_str());

    buf.str("");
    buf << homeDirectory(getpid()) << "/.speedhack/" << getpid();

    outputMemory = new ofstream(buf.str().c_str());
    if (outputMemory->bad())
    {
        outputMemory = 0;
        return;
    }
    (*outputMemory) << &M << std::endl << &N << std::endl;
    outputMemory->close();
}

void resetValues()
{
    if (timezero && gettimeofday_orig)
    {
        timeval tv;
        gettimeofday_orig(&tv,NULL);
        tv.tv_sec = M*tv.tv_sec - M*timezero->tv_sec + N*timezero->tv_sec;
        tv.tv_usec = M*tv.tv_usec - M*timezero->tv_usec + N*timezero->tv_usec;
        tv.tv_usec += ((tv.tv_sec % N) * 1000000);
        tv.tv_sec /= N;
        tv.tv_usec /= N;
        while(tv.tv_usec >= 1000000)
        {
            tv.tv_usec -= 1000000;
            tv.tv_sec += 1;
        }
        while(tv.tv_usec < 0)
        {
            tv.tv_usec += 1000000;
            tv.tv_sec -= 1;
        }
        offsettv.tv_sec = lasttv.tv_sec - tv.tv_sec;
        offsettv.tv_usec = lasttv.tv_usec - tv.tv_usec;
    }
    M_old = M;
    N_old = N;
}

extern "C" int gettimeofday(timeval *tv, class timezone *tz)
{
    if (M_old != M || N_old != N)
        resetValues();
    int val;
    gettimeofday_orig=(go)dlsym(RTLD_NEXT,"gettimeofday");
    // Output memory is a file to show where is int M and int N in memory,
    // so you can modify speed hack in run time without the need of recompile library:
    if (!outputMemory)
        outputMandN();
    if (!timezero)
    {
        timezero = new timeval;
        val = gettimeofday_orig(timezero,tz);
        (*tv) = (*timezero);
        return val;
    }

    val = gettimeofday_orig(tv,tz);
    // Multiply the seconds:
    tv->tv_sec = M*tv->tv_sec - M*timezero->tv_sec + N*timezero->tv_sec;
    // Multiply the microseconds:
    tv->tv_usec = M*tv->tv_usec - M*timezero->tv_usec + N*timezero->tv_usec;
    // Add the modulus of seconds to microseconds:
    tv->tv_usec += ((tv->tv_sec % N) * 1000000);
    tv->tv_sec /= N;
    tv->tv_usec /= N;
    tv->tv_sec += offsettv.tv_sec;
    tv->tv_usec += offsettv.tv_usec;
    while(tv->tv_usec >= 1000000)
    {
        tv->tv_usec -= 1000000;
        tv->tv_sec += 1;
    }
    while(tv->tv_usec < 0)
    {
        tv->tv_usec += 1000000;
        tv->tv_sec -= 1;
    }
    lasttv = (*tv);
    return val;
}
