#include <stdlib.h>
// Xeat Engine V0.01
// lilezek (lilezek@gmail.com)

#include <stdlib.h>
#include <iostream>
#include "misc.h"
#include "memory.h"
#include "speedhack.h"

int mainSpeedHack(int argc, char ** argv);
int mainCheckFlags(int argc, char ** argv);
int main(int argc, char ** argv);

using namespace std;

int main(int argc, char ** argv)
{
    // Check for super user rights:
    setuid(0);
    if (getuid() != 0)
    {
        cout << "You must be super user to use this program" << endl;
        return 0;
    }
    cout << "Xeat Engine V0.04" << endl;
    cout << "1) Modify speedhack a LD_PRELOADED app" << endl;
    cout << "2) Check map readable and writeable regions of a program" << endl;
    int c;
    cin >> c;
    if (c == 1)
        return mainSpeedHack(argc,argv);
    else if (c == 2)
        return mainCheckFlags(argc,argv);

    return 0;
}

int mainCheckFlags(int argc, char ** argv)
{
    pid_t pid;
    vector<s_map> maps;

    cout << "Enter the PID of the program you want to get memory regions" << endl;
    cin >> pid;

    maps = writeableRegions(pid);
    for (unsigned int i = 0; i < maps.size(); i++)
        cout << maps[i].start << " " << maps[i].end << " " << maps[i].perms << endl;
    return 0;
}

int mainSpeedHack(int argc, char ** argvs)
{
    pid_t pid;
    int m,n;
    cout << "Enter the PID of the program you want speedhack" << endl;
    cin >> pid;
    getSpeed(pid,&m,&n);
    cout << "Original speed: " << m << "/" << n << endl;
    cout << "Enter m/n speed" << endl;
    cin >> m >> n;
    setSpeed(pid,m,n);
    return 0;
}
