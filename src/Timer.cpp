
#include "Timer.h"
//#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

// it_interval指定间隔时间，it_value指定初始定时时间。如果只指定it_value，就是实现一次定时；
//如果同时指定 it_interval，则超时后，系统会重新初始化it_value为it_interval，实现重复定时；两者都清零，则会清除定时器。
// tv_sec提供秒级精度，tv_usec提供微秒级精度，以值大的为先，注意1s = 1000000。
void SetTimer(int interval,void(*pfunc)(int))
{
	signal(SIGALRM, pfunc);

        struct itimerval time;
       	time.it_value.tv_sec = 0;
       	time.it_value.tv_usec = interval;

       	time.it_interval.tv_sec = 0;
       	time.it_interval.tv_usec = interval;

        setitimer(ITIMER_REAL, &time,NULL);
}



