#include "src/DeathClock.h"

uv_mutex_t DEATH_CLOCK_IO_MUTEX;

int main(int argc, char *argv[]) 
{

    int status = uv_mutex_init(&DEATH_CLOCK_IO_MUTEX);
    if (status != 0) {
        cout << "unable to initialize mutex" << endl;
        exit(1);
    }

    DeathClock aDeathClock(1.2, "sample main function 1");
    DeathClock bDeathClock(1.1, "sample main function 2");
    DeathClock cDeathClock(1, "sample main function 3");
    usleep(2000000); // sleep for 2sec, sample main function 3 explosions!
    cout << "never get here" << endl;
    return 0;
}