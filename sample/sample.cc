#include "DeathClock.h"

int main(int argc, char *argv[]) 
{
    string cleanupPath(""); // nothing to cleanup
    DeathClock rDeathClock(1, "sample main function", cleanupPath);
    usleep(1100000); // sleep for 1.1sec, explosions!
    cout << "never get here" << endl;
    return 0;
}