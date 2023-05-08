#include <time.h>
#include <stdio.h>

class Timer
{
private:
    time_t t_start, t_end, t_elapse;
    struct tm * t_report;
    char t_buffer[100];
    
public:
    Timer();
    ~Timer();

    char * start();
    char * end();
    char * report();
};

Timer::Timer() {
    t_start = time(NULL);
}

Timer::~Timer() {

}

char * Timer::start() 
{
    t_start = time(NULL);
    sprintf(t_buffer, "Start time: %s", ctime(&t_start));

    return t_buffer;
}

char * Timer::end() 
{
    t_end = time(NULL);
    sprintf(t_buffer, "End time: %s", ctime(&t_end));

    return t_buffer;
}


char * Timer::report() 
{
    t_elapse = (time_t)difftime(t_end, t_start);
    t_report = gmtime(&t_elapse);

    sprintf(t_buffer, "Elapsed time: %d day(s), %d hour(s), %d min(s), %d sec(s).\n",
	        t_report->tm_yday, 
	        t_report->tm_hour, 
	        t_report->tm_min,
	        t_report->tm_sec);

    return t_buffer;
}


