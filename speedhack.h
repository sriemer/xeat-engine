// Speed hack module
#include <sys/types.h>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <pwd.h>

using namespace std;

// Sets m/n speedhack to a speedhacked program.
void setSpeed(pid_t target, int m, int n);

// Gets m/n speedhack of a speedhacked program.
void getSpeed(pid_t target, int * m, int * n);
