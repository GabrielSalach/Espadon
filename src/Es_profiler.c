#include "../Espadon.h"

Es_Profiler* es_profiler_create() {
    Es_Profiler* rtrn_profiler;
    rtrn_profiler = malloc(sizeof(Es_Profiler));
    return rtrn_profiler;
}

void es_profiler_start(Es_Profiler* p) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &p->starting_time);
}

void es_profiler_stop(Es_Profiler* p) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &p->ending_time);
}

double es_profiler_get_elapsed_time(Es_Profiler* p) {
    timespec temp;
    temp = es_time_diff(p->starting_time, p->ending_time);
    p->elapsed_time = (double) temp.tv_sec + (double) temp.tv_nsec/ES_NANOSEC;                           
    return p->elapsed_time;
}

uint16_t es_get_fps() {
    return (uint16_t) 1/es_delta_time();
}