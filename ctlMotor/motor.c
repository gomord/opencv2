
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)
#define UP_MOTOR 0
#define DOWN_MOTOR 1
#define LEFT_MOTOR 2
#define RIGHT_MOTOR 3
#define INT_NSEC 400
#define MAX_TIMERS 4
int g_max_x;
int g_max_y;
typedef void (*timer_callback_t)(int); // Hide the ugliness
timer_t timerid[MAX_TIMERS];
struct sigevent sev[MAX_TIMERS];
unsigned char GPIO[MAX_TIMERS];
void * callback_timers[MAX_TIMERS];
static void
handler(int sig, siginfo_t *si, void *uc)
{
	/* Note: calling printf() from a signal handler is not
	   strictly correct, since printf() is not async-signal-safe;
	   see signal(7) */
	int timer;

	timer_callback_t tcl;
	timer = si->si_value.sival_int;
	printf("Caught signal %d\n", sig);
	tcl = callback_timers[timer];
	tcl(timer);
//	(timer_create*)callback_timers[timer](timer);
}
void defualt_callback(int timer){
	printf("// turn off gpio\n %d",timer);
}
static int init_timers(){
	int i;
	struct sigaction sa;
	/* Establish handler for timer signal */

	printf("Establishing handler for signal %d\n", SIG);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) == -1)
		errExit("sigaction");

	for(i=0; i<MAX_TIMERS;i++){
		sev[i].sigev_notify = SIGEV_SIGNAL;
		sev[i].sigev_signo = SIG;
		sev[i].sigev_value.sival_int = i;
		callback_timers[i] = defualt_callback;
		if (timer_create(CLOCKID, &sev[i], &timerid[i]) == -1)
			errExit("timer_create");


	}
	return 0;
}
static int set_timer(int timer,int sec,int nsec){
	struct itimerspec its;
	its.it_value.tv_sec = sec;
	its.it_value.tv_nsec =nsec;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec =0;
	if (timer_settime(timerid, 0, &its, NULL) == -1)
		errExit("timer_settime");
	return 0;

}

int init_motor(int max_x,int max_y){
	g_max_x = max_x;
	g_max_y = max_y;	
	init_timers();
}
int set_motor(int x,int y){
	if(x > g_max_x*3/4){
		//to dogpio set
		set_timer(LEFT_MOTOR,0,INT_NSEC);
	}
	if(x < g_max_x*1/4){
		//to dogpio set
		set_timer(RIGHT_MOTOR,0,INT_NSEC);
	}
	if(y > g_max_y*3/4){
		//to dogpio set
		set_timer(UP_MOTOR,0,INT_NSEC);
	}
	if(y > g_max_y*1/4){
		//to dogpio set
		set_timer(DOWN_MOTOR,0,INT_NSEC);
	}
}
int main(){
	char c;
	while(c != 27){
		c = getchar();
		printf("%c\n",c);
	}

}
int exit_timers(){
	int i;
	for(i=0; i<MAX_TIMERS;i++){
	}
	return 0;
	
}
