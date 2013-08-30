#include "DeathClock.h"

#define BEGIN_ASYNC(_data, async, after) \
uv_work_t *_req = new uv_work_t; \
_req->data = _data; \
uv_queue_work(uv_default_loop(), _req, async, (uv_after_work_cb)after);
typedef void async_rtn;
#define RETURN_ASYNC
#define RETURN_ASYNC_AFTER delete req;


extern uv_mutex_t DEATH_CLOCK_IO_MUTEX;
unordered_map<unsigned int,int> DEATH_CLOCK_MAP;
unsigned long DEATH_CLOCK_ID = 0;

async_rtn asyncDeathClock(uv_work_t *req)
{
    DeathClockData *m = (DeathClockData *)req->data;

    unordered_map<unsigned int, int>::const_iterator pContinueCountDown = DEATH_CLOCK_MAP.find(m->m_clockID);
    if (pContinueCountDown == DEATH_CLOCK_MAP.end()) {
        cout << "error finding id " << m->m_clockID << " in map exiting ";
        exit(1);
    }
    int continueCountDown = pContinueCountDown->second;


    while (continueCountDown) {
        uv_mutex_lock(&DEATH_CLOCK_IO_MUTEX); 
        cout << "checking for " << m->m_sErrorMessage << " counter " << m->m_Counter << " max allowed " << m->m_NMaxChecks << endl;
        uv_mutex_unlock(&DEATH_CLOCK_IO_MUTEX); 
        
        if (m->m_Counter >= m->m_NMaxChecks) {           
            uv_mutex_lock(&DEATH_CLOCK_IO_MUTEX);        
            DEATH_CLOCK_MAP[m->m_clockID] = 0;
            
            stringstream err;
            err << " ERROR DeathClock::DeathClock end of universe reached, ---<<<*** EXPLOSIONS ***>>>--- message: " << m->m_sErrorMessage;
            cout << err.str() << endl;        
            uv_mutex_unlock(&DEATH_CLOCK_IO_MUTEX);

            assert(false); // create abort, main loop won't receive any messages if it's frozen
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

void asyncDeathClock(double TimeOutFailureSeconds, const string &sErrorMessage, unsigned int clockID, int uSecSleep)
{
    DeathClockData *pm = new DeathClockData;
    pm->m_Counter = 0;
    pm->m_uSecSleep = uSecSleep;
    pm->m_NMaxChecks = TimeOutFailureSeconds / ((double)uSecSleep * 1e-6);
    pm->m_sErrorMessage = sErrorMessage.c_str();
    pm->m_clockID = clockID;

    BEGIN_ASYNC(pm, asyncDeathClock, afterDeathClock);
}


DeathClock::DeathClock(double TimeOutFailureSeconds, const string &sErrorMessage, int uSecSleep) :
    m_uSecSleep(uSecSleep),
    m_sErrorMessage(sErrorMessage)
{
    m_ID = DEATH_CLOCK_ID++;    
    DEATH_CLOCK_MAP[m_ID] = 1;
    asyncDeathClock(TimeOutFailureSeconds,sErrorMessage,m_ID,uSecSleep);
}

DeathClock::~DeathClock() 
{
    stopDeathClock();
}

void DeathClock::stopDeathClock() 
{ 
    DEATH_CLOCK_MAP[m_ID] = 0;
};