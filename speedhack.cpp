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


using std::ofstream;
using std::string;

static timeval * timezero = 0;
static timespec * timespcz = 0;
static timeval lasttv;
static timespec lastpz;
static timeval offsettv;
static timespec offsetts;
static ofstream * outputMemory = 0;

// Output memory is a file to show where is int M and int N in memory,
// so you can modify speed hack in run time without the need of recompile library:
typedef int (*go)(timeval *tv, class timezone *tz);
typedef int (*cg)(clockid_t clckid, timespec * ts);

static go gettimeofday_orig = 0;
static cg clock_gettime_orig = 0;

// Multiply speed:
int M = 4;
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
    const char *homedir = pw->pw_dir;

    string s = homedir;

    // Adding some information to directory:
    std::stringstream buf;
    buf << getpid();
    s += "/.speedhack/";
    s += buf.str();
    outputMemory = new ofstream(s.c_str());
    if (outputMemory->bad())
    {
        outputMemory = 0;
        return;
    }
    (*outputMemory) << &M << std::endl << &N << std::endl;
    outputMemory->close();
}

void resetValues(clockid_t clckid)
{
    if (clckid != -1 && timespcz && clock_gettime_orig)
    {
        clock_gettime_orig(clckid,&offsetts);
        offsetts.tv_sec = lastpz.tv_sec - offsetts.tv_sec;
        offsetts.tv_nsec = lastpz.tv_nsec - offsetts.tv_nsec;
    }
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

extern "C" int clock_gettime(clockid_t clckid, timespec * ts)
{
    if (M_old != M || N_old != N)
        resetValues(clckid);
    int val;
    clock_gettime_orig=(cg)dlsym(RTLD_NEXT,"clock_gettime");
    // Output memory is a file to show where is int M and int N in memory,
    // so you can modify speed hack in run time without the need of recompile library:
    if (!outputMemory)
        outputMandN();
    if (!timespcz)
    {
        timespcz = new timespec;
        val = clock_gettime_orig(clckid,timespcz);
        (*ts) = (*timespcz);
        return val;
    }
    val = clock_gettime_orig(clckid,ts);
    // Multiply the seconds:
    ts->tv_sec = M*ts->tv_sec - M*timespcz->tv_sec + N*timespcz->tv_sec + offsetts.tv_sec;
    // Multiply the microseconds:
    ts->tv_nsec = M*ts->tv_nsec - M*timespcz->tv_nsec + N*timespcz->tv_nsec + offsetts.tv_sec;
    // Add the modulus of seconds to microseconds:
    ts->tv_nsec += ((ts->tv_sec % N) * 1000000000);
    ts->tv_sec /= N;
    ts->tv_nsec /= N;
    while(ts->tv_nsec >= 1000000000)
    {
        ts->tv_nsec -= 1000000000;
        ts->tv_sec += 1;
    }
    while(ts->tv_nsec < 0)
    {
        ts->tv_nsec += 1000000000;
        ts->tv_sec -= 1;
    }
    lastpz = (*ts);
    return val;
}

extern "C" int gettimeofday(timeval *tv, class timezone *tz)
{
    if (M_old != M || N_old != N)
        resetValues(-1);
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
