#include "../Espadon.h"

struct Es_Profiler {
    struct timespec starting_time;                                                      /* Timestamp set when using es_profiler_start() method. */
    struct timespec ending_time;                                                        /* Timestamp set when using es_profiler_stop() method. */
    double elapsed_time;                                                                /* Difference between ending_time and starting_time. */
};