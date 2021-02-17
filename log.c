#include <stdlib.h>
#include <stdio.h>
#include "input.h"
#include "simulation.h"

extern struct sim_params settings;

void log_record(struct event *e, FILE *f) {
  if(e->time < settings.init_time) 
    return;
  switch(e->et) {
    case JOB_ARRIVE:
      fprintf(f, "JOB_ARRIVE");
      break;
    case JOB_START:
      fprintf(f, "JOB_START");
      break;
    case DISK1_START:
      fprintf(f, "DISK1_START");
      break;
    case DISK2_START:
      fprintf(f, "DISK2_START");
      break;
    case NET_START:
      fprintf(f, "NET_START");
      break;
    case NET_END:
      fprintf(f, "NET_END");
      break;
    case JOB_END:
      fprintf(f, "JOB_END");
      break;
    case DISK1_END:
      fprintf(f, "DISK1_END");
      break;
    case DISK2_END:
      fprintf(f, "DISK2_END");
      break;
    case NET_QUEUE:
      fprintf(f, "NET_QUEUE");
      break;
    case JOB_QUEUE:
      fprintf(f, "JOB_QUEUE");
      break;
    case DISK1_QUEUE:
      fprintf(f, "DISK1_QUEUE");
      break;
    case DISK2_QUEUE:
      fprintf(f, "DISK2_QUEUE");
      break;
    case SIMULATION_END:
      fprintf(f, "SIMULATION_END");
    default:
      break;
  }
  fprintf(f, " %ld\n", e->time);
}
