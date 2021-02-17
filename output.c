#include <stdlib.h>
#include <stdio.h>
#include "input.h"
#include "stats.h"
#include "output.h"

extern struct sim_params settings;
extern struct stats simulation_stats;

void write_stats_to_file(FILE *f) {
  double queue_avg[NUM_RES];
  long queue_max[NUM_RES];
  double util_percent[NUM_RES];
  int num_jobs = simulation_stats.jobs_completed;
  long total_duration = settings.fin_time - settings.init_time;

  for(int i = 0; i < NUM_RES; i++) {
    queue_max[i] = simulation_stats.queue_max[i];
    queue_avg[i] = ((double) simulation_stats.queue_units[i]) / total_duration;
    util_percent[i] = ((double) simulation_stats.util_units[i]) / total_duration * 100;
  }

  fprintf(f, "Jobs completed: %d\n", num_jobs);
  fprintf(f, "%s\n", "Queue Max:\n");
  fprintf(f, "%s %ld\n", "CPU:", queue_max[CPU]);
  fprintf(f, "%s %ld\n", "DISK1:", queue_max[DISK1]);
  fprintf(f, "%s %ld\n", "DISK2:", queue_max[DISK2]);
  fprintf(f, "%s %ld\n", "NET:", queue_max[NET]);
  
  fprintf(f, "\n%s\n", "Queue Average:\n");
  fprintf(f, "%s %f\n", "CPU:", queue_avg[CPU]);
  fprintf(f, "%s %f\n", "DISK1:", queue_avg[DISK1]);
  fprintf(f, "%s %f\n", "DISK2:", queue_avg[DISK2]);
  fprintf(f, "%s %f\n", "NET:", queue_avg[NET]);

  fprintf(f, "\n%s\n", "Util Percent:\n");
  fprintf(f, "%s %f\n", "CPU:", util_percent[CPU]);
  fprintf(f, "%s %f\n", "DISK1:", util_percent[DISK1]);
  fprintf(f, "%s %f\n", "DISK2:", util_percent[DISK2]);
  fprintf(f, "%s %f\n", "NET:", util_percent[NET]);  
}
