#ifndef STATS_INCLUDE
#define STATS_INCLUDE

#include "simulation.h"

enum res {
  CPU,
  DISK1,
  DISK2,
  NET,
  NUM_RES
};

struct stats {
  long queue_units[NUM_RES];
  long queue_max[NUM_RES];
  long util_units[NUM_RES];
  long jobs_completed;
  long total_time;
};

void stats_record(struct event *, struct sim_state *, long last_time, long res_delay);

#endif
