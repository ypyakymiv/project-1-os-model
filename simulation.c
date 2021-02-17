#include <stdlib.h>
#include "utils.h"
#include "priority_heap.h"
#include "input.h"
#include "output.h"
#include "simulation.h"
#include "stats.h"
#include "log.h"

#define SETTINGS_FILE "sim.conf"
#define LOG_FILE "log.txt"
#define STATS_FILE "stats.txt"

extern struct sim_params settings;

long select_from_range(int a, int b) {
  int range = b - a + 1;
  return (rand() % range) + a;
}

int test_prob(double prob) {
  if(rand() < prob * RAND_MAX) return 1;
  else return 0;
}

long job_arrival_delay() {
  return select_from_range(settings.arrive_min, settings.arrive_max);
}

long cpu_time() {
  return select_from_range(settings.cpu_min, settings.cpu_max);
}

long disk1_read() {
  return select_from_range(settings.disk1_min, settings.disk1_max);
}

long disk2_read() {
  return select_from_range(settings.disk2_min, settings.disk2_max);
}

long net_read() {
  return select_from_range(settings.net_min, settings.net_max);
}

int should_quit() {
  return test_prob(settings.quit_prob);
}

int should_network() {
  return test_prob(settings.net_prob);
}

int get_event_priority(void *e) {
  return ((struct event *) e)->time;
}

int main(int argc, char **argv) {
  read_params(SETTINGS_FILE);
  FILE *log_file = ec_fopen(LOG_FILE, "w");
  FILE *stats_file = ec_fopen(STATS_FILE, "w");

  srand(settings.seed);

  struct sim_state state;
  sim_state_init(&state);
  long last_time = 0;
  long res_delay;
  
  struct heap *events = heap_init(READ_BUFFER_SZ, get_event_priority);
  heap_insert(events, alloc_event(SIMULATION_END, settings.fin_time));
  heap_insert(events, alloc_event(JOB_ARRIVE, job_arrival_delay()));

  while(events->curr_sz) {
    struct event *curr_event = heap_pop(events);
    switch(curr_event->et) {
      case JOB_ARRIVE:
        res_delay = job_arrival_delay();
        heap_insert(events, alloc_event(JOB_ARRIVE, curr_event->time + res_delay));
      case JOB_QUEUE:
        if(!state.cpu_load)
          heap_insert(events, alloc_event(JOB_START, curr_event->time));        
        state.cpu_load++;
        break;
      case JOB_START:
        res_delay = cpu_time();
        if(should_quit()) {
          heap_insert(events, alloc_event(JOB_END, curr_event->time + res_delay));
        } else if(should_network()) {
          heap_insert(events, alloc_event(NET_QUEUE, curr_event->time + res_delay));
        } else {
          if(state.disk2_load >= state.disk1_load) {
            heap_insert(events, alloc_event(DISK1_QUEUE, curr_event->time + res_delay));
          } else {
            heap_insert(events, alloc_event(DISK2_QUEUE, curr_event->time + res_delay));
          }
        }
        break;
      case JOB_END:
        state.cpu_load--;
        if(state.cpu_load)
          heap_insert(events, alloc_event(JOB_START, curr_event->time));        
        break;
      case NET_QUEUE:
        state.cpu_load--;
        if(state.cpu_load)
          heap_insert(events, alloc_event(JOB_START, curr_event->time));        
        if(!state.net_load)
          heap_insert(events, alloc_event(NET_START, curr_event->time));
        state.net_load++;
        break;
      case NET_START:
        res_delay = net_read();
        heap_insert(events, alloc_event(NET_END, curr_event->time + res_delay));
        break;
      case NET_END:
        heap_insert(events, alloc_event(JOB_QUEUE, curr_event->time));
        state.net_load--;
        if(state.net_load) {
          heap_insert(events, alloc_event(NET_START, curr_event->time));        
        }
        break;
      case DISK1_QUEUE:
        state.cpu_load--;       
        if(state.cpu_load)
          heap_insert(events, alloc_event(JOB_START, curr_event->time));        
        if(!state.disk1_load) {
          heap_insert(events, alloc_event(DISK1_START, curr_event->time));
        }
        state.disk1_load++;
        break;
      case DISK1_START:
        res_delay = disk1_read();
        heap_insert(events, alloc_event(DISK1_END, curr_event->time + res_delay));
        break;
      case DISK1_END:
        state.disk1_load--;
        if(state.disk1_load)
          heap_insert(events, alloc_event(DISK1_START, curr_event->time));
        heap_insert(events, alloc_event(JOB_QUEUE, curr_event->time));
        break;
      case DISK2_QUEUE:
        state.cpu_load--;
        if(state.cpu_load)
          heap_insert(events, alloc_event(JOB_START, curr_event->time));        
        if(!state.disk2_load) {
          heap_insert(events, alloc_event(DISK2_START, curr_event->time));
        }
        state.disk2_load++;
        break;
      case DISK2_START:
        res_delay = disk2_read();
        heap_insert(events, alloc_event(DISK2_END, curr_event->time + res_delay));
        break;
      case DISK2_END:
        state.disk2_load--;
        if(state.disk2_load)
          heap_insert(events, alloc_event(DISK2_START, curr_event->time));
        heap_insert(events, alloc_event(JOB_QUEUE, curr_event->time));
        break;
      case SIMULATION_END:
        goto EXIT_SIM_LOOP;        
      
    }
    log_record(curr_event, log_file);
    stats_record(curr_event, &state, last_time, res_delay);    
    last_time = curr_event->time;
    free(curr_event);
  }

  EXIT_SIM_LOOP:
  write_stats_to_file(stats_file);
  return 0;
//  heap_free(events, free_event);
}

void sim_state_init(struct sim_state *s) {
  s->time = 0;
  s->cpu_load = 0;
  s->disk1_load = 0;
  s->disk2_load = 0;
  s->net_load = 0;
}

struct event *alloc_event(event_type et, long time) {
  struct event *new_event = ec_malloc(sizeof(struct event));
  new_event->et = et;
  new_event->time = time;
  return new_event;
}

void free_event(void *e) {
  free(e);
}

