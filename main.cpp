#include <stdlib.h>
// Xeat Engine V0.05
// lilezek (lilezek@gmail.com)

#include <stdlib.h>
#include <iostream>
#include "misc.h"
#include "memory.h"
#include "speedhack.h"
#include "reader.h"

int mainSpeedHack(int argc, char ** argv);
int mainCheckFlags(int argc, char ** argv);
int mainSnapshot(int argc, char ** argv);
int mainSearchLong(int argc, char ** argv);
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
    cout << "Xeat Engine V0.05" << endl;
    cout << "1) Modify speedhack a LD_PRELOADED app" << endl;
    cout << "2) Check map readable and writeable regions of a program" << endl;
    cout << "3) Do an snapshot of a program" << endl;
    cout << "4) Search for a long in a program" << endl;
    int c;
    cin >> c;
    if (c == 1)
        return mainSpeedHack(argc,argv);
    else if (c == 2)
        return mainCheckFlags(argc,argv);
    else if (c == 3)
        return mainSnapshot(argc,argv);
    else if (c == 4)
        return mainSearchLong(argc,argv);
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

int mainSpeedHack(int argc, char ** argv)
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

int mainSnapshot(int argc, char ** argv)
{
    pid_t pid;
    int m;
    cout << "Enter the PID of the program you want to create an snapshot" << endl;
    cin >> pid;
    FILE * sn = snapshot(pid);
    fseek(sn,0,SEEK_END);
    size_t size = ftell(sn);
    cout << "Snapshot finished with the size of " << size << " bytes. Write anything to close" << endl;
    cin >> m;
    return 0;
}

int mainSearchLong(int argc, char ** argv)
{
    pid_t pid;
    any_value m(0L);    // LONG VALUE
    long ocr = 0;       // How much values have found.
    cout << "Enter the PID of the program you want look for the long" << endl;
    cin >> pid;
    cout << "Enter the value of the long you want to look for" << endl;
    cin >> *(long*)m.getPointer();
    //reader r(NULL,true,pid,true);
    FILE * found;
    found = searchValue(pid,NULL,false,m,EXACT,&ocr);
    fclose(found);
    cout << ocr << " values match that number" << endl;
    return 0;
}

