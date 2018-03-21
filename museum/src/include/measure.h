#include <sys/time.h>
#include <unistd.h>

// Claramente no es threadsafe

#define MAX_COUNTERS 100

static struct timeval times[MAX_COUNTERS];
static int activeCounter=0;

void tic(){
    if(activeCounter < MAX_COUNTERS){
        gettimeofday(&times[activeCounter], NULL);
        activeCounter++;
    }
    //Silently fail...
    return;
}

double toc(){
    if(activeCounter < 0)
        return 0; //silently fail again...
    activeCounter--;
    
    struct timeval endTime;
    gettimeofday(&endTime, NULL);
    struct timeval startTime = times[activeCounter];
    
    double elapsedTime = (endTime.tv_sec - startTime.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (endTime.tv_usec - startTime.tv_usec) / 1000.0;   // us to ms
    return elapsedTime;
}
