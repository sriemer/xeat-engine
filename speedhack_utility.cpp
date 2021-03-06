#include "speedhack.h"
#include "memory.h"
#include "misc.h"

// Sets m/n speedhack to a speedhacked program.
void setSpeed(pid_t target, int m, int n)
{
    stringstream sstr;
    sstr.str("");
    sstr << homeDirectory(target) << "/.speedhack/" << target;

    attach(target);

    ifstream spd(sstr.str().c_str());
    long int pos;
    // First value:
    spd >> hex >> pos;
    writeregion(target,&m,sizeof(int),pos);
    // Second value:
    spd >> hex >> pos;
    writeregion(target,&n,sizeof(int),pos);
    detach(target);
}

// Gets m/n speedhack of a speedhacked program.
void getSpeed(pid_t target, int * m, int * n)
{
    stringstream sstr;
    sstr.str("");
    sstr << homeDirectory(target) << "/.speedhack/" << target;

    attach(target);

    cout << sstr.str().c_str() << endl;

    ifstream spd(sstr.str().c_str());
    long int pos;
    // First value:
    spd >> hex >> pos;
    readregion(target,m,sizeof(int),pos);
    // Second value:
    spd >> hex >> pos;
    readregion(target,n,sizeof(int),pos);
    detach(target);
}
