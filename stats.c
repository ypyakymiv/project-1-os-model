#include <stdlib.h>
#include "input.h"
#include "simulation.h"
#include "stats.h"

extern struct sim_params settings;

struct stats simulation_stats;

long in_range(long from, long to) {
  if(from < settings.init_time && from < settings.fin_time) from = settings.init_time;
  if(to > settings.fin_time && to > settings.init_time) to = settings.fin_time;
  return to - from;
}

void stats_record(struct event *e, struct sim_state *state, long last_time, long res_delay) {
  if(e->time < settings.init_time) return;
  long elapsed = e->time - last_time;
  switch(e->et) {
    case JOB_START:
      simulation_stats.util_units[CPU] += in_range(e->time, e->time + res_delay);
      break;
    case DISK1_START:
      simulation_stats.util_units[DISK1] += in_range(e->time, e->time + res_delay);
      break;
    case DISK2_START:
      simulation_stats.util_units[DISK2] += in_range(e->time, e->time + res_delay);
      break;
    case NET_START:
      simulation_stats.util_units[NET] += in_range(e->time, e->time + res_delay);
      break;
    case JOB_END:
      simulation_stats.jobs_completed++;
      break;
    default:
      break;
  }
  
  long load[NUM_RES] = { state->cpu_load, state->disk1_load, state->disk2_load, state->net_load };
  for(int i = 0; i < NUM_RES; i++) {
    if(load[i] > 1) {
      if(simulation_stats.queue_max[i] < (load[i] - 1)) simulation_stats.queue_max[i] = load[i] - 1;
      simulation_stats.queue_units[i] += (load[i] - 1) * elapsed;
    }
  }
}
