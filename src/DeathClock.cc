#include "DeathClock.h"

async_rtn asyncDeathClock(uv_work_t *req)
{
    RunTime t1;

    DeathClockData *m = (DeathClockData *)req->data;

    while (m->m_ContinueCountDown) {
        // cout << "checking for " << m->m_sErrorMessage << " counter " << m->m_Counter << " max allowed " << m->m_NMaxChecks << " continue count down " << m->m_ContinueCountDown << " pointer " << (void *)m->m_ContinueCountDown << endl;
        if (m->m_Counter >= m->m_NMaxChecks) {
            m->m_ContinueCountDown = 0;

            stringstream err;
            err << " ERROR DeathClock::DeathClock end of universe reached, ---<<<*** EXPLOSIONS ***>>>--- message: " << m->m_sErrorMessage;
            cout << err.str() << endl;        

            // safe guard your path cleanup area
            std::size_t found = m->m_sPathToClean.find("/tmp/");

            if (found != string::npos) {
                cout << "ABOUT TO KILL JOB IN asyncDeathClock, this path is going bye bye " << m->m_sPathToClean << endl;
                stringstream cleanUp;
                cleanUp << "sudo rm -rf " << m->m_sPathToClean << " &";
                const char *eString = cleanUp.str().c_str();
                system(eString);
            }

            usleep(1000); // sleep for a 1ms, then die
            assert(false); // kill switch, would like a cleaner one to interrupt the main loop and return error state
        }
        m->m_Counter++;
        usleep(m->m_uSecSleep); // usleep for x useconds
    }        

    RETURN_ASYNC
}


async_rtn afterDeathClock(uv_work_t *req) 
{
    DeathClockData *m = (DeathClockData *)req->data;
    if (m) delete m;
    RETURN_ASYNC_AFTER
}

void asyncDeathClock(double TimeOutFailureSeconds, const string &sErrorMessage, int uSecSleep, int &ContinueCountDown, const string &sPathToClean)
{
    // cout << "asyncDeathClock setup: message " << sErrorMessage << " asyncDeathClock pointer to continue count down " << (void *)&ContinueCountDown << endl;
    DeathClockData *pm = new DeathClockData(ContinueCountDown);
    pm->m_Counter = 0;
    pm->m_uSecSleep = uSecSleep;
    pm->m_NMaxChecks = TimeOutFailureSeconds / ((double)uSecSleep * 1e-6);
    pm->m_sErrorMessage = sErrorMessage.c_str();
    pm->m_sPathToClean = sPathToClean.c_str();

    BEGIN_ASYNC(pm, asyncDeathClock, afterDeathClock);
}


DeathClock::DeathClock(double TimeOutFailureSeconds, const string &sErrorMessage, const string &sPathToClean, int uSecSleep) :
    m_uSecSleep(uSecSleep),
    m_ContinueCountDown(1),
    m_sErrorMessage(sErrorMessage)
{
    asyncDeathClock(TimeOutFailureSeconds,sErrorMessage,uSecSleep,m_ContinueCountDown,sPathToClean);
}

DeathClock::~DeathClock() 
{
    stopDeathClock();
}