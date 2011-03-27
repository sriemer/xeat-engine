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
    // Check for super user rights:
    setuid(0);
    if (getuid() != 0)
    {
        cout << "You must be super user to use this program" << endl;
        return 0;
    }
    pid_t pid;
    int m,n;
    cout << "Xeat Engine V0.03" << endl;
    cout << "Enter the PID of the program you want speedhack" << endl;
    cin >> pid;
    getSpeed(pid,&m,&n);
    cout << "Original speed: " << m << "/" << n << endl;
    cout << "Enter m/n speed" << endl;
    cin >> m >> n;
    setSpeed(pid,m,n);
    return 0;
}
