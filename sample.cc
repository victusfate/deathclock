#include "src/DeathClock.h"

uv_mutex_t DEATH_CLOCK_IO_MUTEX;

int main(int argc, char *argv[]) 
{

    int status = uv_mutex_init(&DEATH_CLOCK_IO_MUTEX);
    if (status != 0) {
        cout << "unable to initialize mutex" << endl;
        exit(1);
    }

    string cleanupPath(""); // nothing to cleanup
    DeathClock aDeathClock(10, "sample main function 1", cleanupPath);
    DeathClock bDeathClock(10, "sample main function 2", cleanupPath);
    DeathClock cDeathClock(10, "sample main function 3", cleanupPath);
    usleep(11000000); // sleep for 11sec, explosions!
    cout << "never get here" << endl;
    return 0;
}