#include <stdlib.h>
// Xeat Engine V0.01
// lilezek (lilezek@gmail.com)

#include <stdlib.h>
#include <iostream>
#include "misc.h"
#include "memory.h"
#include "speedhack.h"

using namespace std;

int main(int argc, char ** argv)
{
    setSpeedFolder("/home/lilezek/.speedhack/");
    // Check for super user rights:
    setuid(0);
    if (getuid() != 0)
    {
        cout << "You must be super user to use this program" << endl;
        return 0;
    }
    pid_t pid;
    int mandn[2];
    cout << "Xeat Engine V0.02" << endl;
    cout << "Enter the PID of the program you want to speedhack" << endl;
    cin >> pid;
    cout << "Enter the m and n of the speedhack" << endl;
    while (true)
    {
        cin >> mandn[0] >> mandn[1];
        setSpeed(pid,mandn[0],mandn[1]);
    }
    return 0;
}
