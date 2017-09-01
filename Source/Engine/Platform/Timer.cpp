
#include <giga-engine.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#ifdef WIN32

#define CLOCK_REALTIME	0

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

int clock_gettime(int, struct timespec *spec) {
    __int64 wintime;
    
    GetSystemTimeAsFileTime((FILETIME*)&wintime);
    
    //1 jan 1601 to 1 jan 1970
    wintime -= 116444736000000000i64;
    spec->tv_sec = wintime / 10000000i64;           //seconds
    spec->tv_nsec = wintime % 10000000i64 * 100;    //nano-seconds
    
    return 0;
}
#else
#include <unistd.h>
#endif

void Timer::GetTimestamp(struct timespec *ts) {
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, ts);
#endif
}

Timer::Timer() {
    memset(&m_startTime, 0, sizeof(timespec));
    memset(&m_lastTime, 0, sizeof(timespec));
}

Timer::~Timer() {
    
}

void Timer::Start() {
    Timer::GetTimestamp(&m_startTime);
}

float Timer::Duration() {
    timespec ts;
    Timer::GetTimestamp(&ts);
    
    float seconds, nanoseconds;
    timespec delta = Diff(&m_startTime, &ts);
    
    return(delta.tv_sec + (delta.tv_nsec / 1000000000.0f));
}

timespec Timer::Diff(timespec* start, timespec* end) {
    timespec t;
    
    if(end->tv_nsec - start->tv_nsec < 0) {
        t.tv_sec = end->tv_sec - start->tv_sec - 1;
        t.tv_nsec = end->tv_nsec - start->tv_nsec + 1000000000.0f;
    }
    else {
        t.tv_sec = end->tv_sec - start->tv_sec;
        t.tv_nsec = end->tv_nsec - start->tv_nsec;
    }
    return(t);
}

float Timer::Reset() {
    timespec ts;
    Timer::GetTimestamp(&ts);
    
    timespec delta = Diff(&m_startTime, &ts);
    
    m_startTime = ts;
    return(delta.tv_sec + (delta.tv_nsec / 1000000000.0f));
}

void Timer::End() {
    memset(&m_startTime, 0, sizeof(timespec));
}

void Timer::Sleep(int milliseconds) {
#ifdef WIN32
    ::Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}
